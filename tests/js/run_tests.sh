#!/bin/sh
#
# Clint's JavaScript, checked on the build host.
#
# Each case is a script and the output it must produce. An interpreter
# is wrong in ways that still look plausible - a number that prints as
# 1.0000000001, a loop that runs one time too many - so the checks are
# about exact output rather than about "it did not crash".

fail=0

check() {
    if [ "$2" = 0 ]; then
        echo "  [PASS] $1"
    else
        echo "  [FAIL] $1"
        fail=1
    fi
}

# js <source> -> what it logged
js() {
    printf '%s' "$1" | ./jsrun /dev/stdin
}

# expect <name> <source> <expected output>
expect() {
    out=$(js "$2")
    if [ "$out" = "$3" ]; then
        echo "  [PASS] $1"
    else
        echo "  [FAIL] $1"
        echo "         expected: $3"
        echo "         got:      $out"
        fail=1
    fi
}

echo "the language"

expect "arithmetic keeps precedence" \
    'console.log(1 + 2 * 3, (1 + 2) * 3, 7 % 3, 2 ** 10)' \
    "7 9 1 1024"

expect "numbers print the way JavaScript prints them" \
    'console.log(1, 1.5, 0.1 + 0.2, 1e21, -0)' \
    "1 1.5 0.30000000000000004 1e+21 0"

expect "strings and numbers add differently" \
    'console.log(1 + 2, "1" + 2, 1 + "2", "3" * "4")' \
    "3 12 12 12"

expect "comparison is loose or strict as asked" \
    'console.log(1 == "1", 1 === "1", null == undefined, null === undefined)' \
    "true false true false"

expect "a closure keeps its variable" \
    'function counter(){var n=0;return function(){return ++n;};}
     var c = counter(); c(); c(); console.log(c())' \
    "3"

expect "recursion works to a useful depth" \
    'function f(n){return n<2?n:f(n-1)+f(n-2);} console.log(f(20))' \
    "6765"

expect "an arrow function keeps the outer this" \
    'var o = {name:"x", go:function(){ return [1].map(() => this.name)[0]; }};
     console.log(o.go())' \
    "x"

expect "a for loop breaks and continues" \
    'var s=0; for (var i=0;i<10;i++){ if(i===3) continue; if(i===6) break; s+=i; }
     console.log(s)' \
    "12"

expect "for-of walks an array, for-in walks its keys" \
    'var out=[]; for (var v of [10,20]) out.push(v);
     for (var k in [10,20]) out.push(k); console.log(out.join(","))' \
    "10,20,0,1"

expect "switch falls through until it breaks" \
    'var m=0; switch(2){case 1: m=1; break; case 2: m=2; case 3: m+=10; break;
     default: m=99;} console.log(m)' \
    "12"

expect "throw is caught, and finally still runs" \
    'try { throw "boom"; } catch (e) { console.log("caught", e); }
     finally { console.log("finally"); }' \
    "caught boom
finally"

expect "typeof does not complain about what is missing" \
    'console.log(typeof nothing, typeof 1, typeof "s", typeof [], typeof (function(){}))' \
    "undefined number string object function"

expect "a template literal interpolates" \
    'var n = 3; console.log(`n is ${n * 2} and "${"x".toUpperCase()}"`)' \
    'n is 6 and "X"'

echo "the library"

expect "string methods do what they say" \
    'var s = "Hello, World";
     console.log(s.length, s.toUpperCase(), s.indexOf("World"), s.slice(7),
                 s.split(", ").join("|"), "  pad  ".trim())' \
    "12 HELLO, WORLD 7 World Hello|World pad"

expect "array methods do what they say" \
    'var a = [3,1,2];
     console.log(a.map(x => x * 2).join(","), a.filter(x => x > 1).length,
                 a.slice().sort().join(""), a.indexOf(2),
                 a.reduce(function(x,y){return x+y;}, 0))' \
    "6,2,4 2 123 2 6"

expect "Math is there" \
    'console.log(Math.max(3,7,2), Math.floor(-3.5), Math.ceil(3.2),
                 Math.abs(-4), Math.sqrt(144), Math.round(2.5))' \
    "7 -4 4 4 12 3"

expect "parseInt and parseFloat stop where the number does" \
    'console.log(parseInt("42px"), parseFloat("3.5rem"), parseInt("0x1f", 16),
                 isNaN(parseInt("x")))' \
    "42 3.5 31 true"

echo "regular expressions"

expect "replace substitutes groups" \
    'console.log("2026-08-21".replace(/(\d+)-(\d+)-(\d+)/, "$3/$2/$1"))' \
    "21/08/2026"

expect "a global replace replaces every match" \
    'console.log("a  b   c".replace(/\s+/g, " "))' \
    "a b c"

expect "match with /g returns all of them" \
    'console.log("a1b22c333".match(/\d+/g).join("-"))' \
    "1-22-333"

expect "test and exec agree with each other" \
    'var r = /^h(ell)o$/i;
     console.log(r.test("HELLO"), r.exec("hello")[1], r.test("nope"))' \
    "true ell false"

expect "split takes a pattern" \
    'console.log("one1two22three".split(/\d+/).join("|"))' \
    "one|two|three"

echo "the document"

cat > /tmp/clint-js-test.html <<'HTML'
<html><head><title>t</title></head><body>
<h1 id="h">old</h1>
<p class="c">one</p><p class="c">two</p>
<ul id="list"></ul>
<button id="b" onclick="document.getElementById('h').textContent='clicked'">go</button>
<script>
document.getElementById("h").innerHTML = "new <b>title</b>";
var items = ["a","b"];
for (var i = 0; i < items.length; i++) {
  var li = document.createElement("li");
  li.textContent = items[i];
  document.getElementById("list").appendChild(li);
}
console.log("class count:", document.getElementsByClassName("c").length);
console.log("query:", document.querySelector("p.c").textContent);
console.log("title:", document.title);
document.querySelector("p").style.color = "red";
document.getElementById("b").addEventListener("click", function (e) {
  console.log("listener saw", e.target.tagName);
});
</script>
</body></html>
HTML

out=$(./jsrun /tmp/clint-js-test.html -html)
echo "$out" | grep -q "class count: 2" && echo "$out" | grep -q "query: one" &&
    echo "$out" | grep -q "title: t"
check "the document answers the queries a page asks it" $?

echo "$out" | grep -q '<h1 id="h">new <b>title</b></h1>'
check "innerHTML replaces the children with parsed markup" $?

echo "$out" | grep -q '<ul id="list"><li>a</li><li>b</li></ul>'
check "createElement and appendChild build a list" $?

echo "$out" | grep -q '<p style="color:red;" class="c">one</p>'
check "element.style writes through to the style attribute" $?

out=$(./jsrun /tmp/clint-js-test.html -html -click b)
echo "$out" | grep -q "listener saw BUTTON"
check "a click reaches a listener added by a script" $?

echo "$out" | grep -q '<h1 id="h">clicked</h1>'
check "a click reaches an onclick attribute" $?

echo "$out" | grep -q "handler=1"
check "the browser can tell an element has a handler" $?

echo "external scripts"

cat > /tmp/clint-js-test-ext.js <<'JS'
console.log("external ran");
document.getElementById("h").textContent = "from src";
JS

cat > /tmp/clint-js-test-src.html <<HTML
<html><body>
<h1 id="h">old</h1>
<script src="/tmp/clint-js-test-ext.js"></script>
<script>console.log("inline after src ran too")</script>
</body></html>
HTML

out=$(./jsrun /tmp/clint-js-test-src.html -html)
echo "$out" | grep -q "external ran"
check "a <script src> is fetched and run" $?

echo "$out" | grep -q "inline after src ran too"
check "an inline script after a src one still runs, in order" $?

echo "$out" | grep -q '<h1 id="h">from src</h1>'
check "a fetched script can touch the document like an inline one" $?

cat > /tmp/clint-js-test-missing.html <<'HTML'
<html><body>
<script src="/tmp/clint-js-test-does-not-exist.js"></script>
<script>console.log("still ran")</script>
</body></html>
HTML

out=$(./jsrun /tmp/clint-js-test-missing.html -html)
echo "$out" | grep -q "still ran"
check "a script that cannot be fetched costs only itself, not the page" $?

rm -f /tmp/clint-js-test-ext.js /tmp/clint-js-test-src.html /tmp/clint-js-test-missing.html

echo "limits"

expect "a runaway loop is stopped rather than obeyed" \
    'for (;;) {} console.log("never")' \
    "error: the script ran for too long and was stopped"

expect "a script that fails leaves a message, not a crash" \
    'null.x' \
    "error: cannot read a property of nothing"

echo "default parameters"

expect "a default is used when an argument is omitted" \
    'function f(a = 5) { console.log(a); } f();' \
    "5"

expect "a default is used when an argument is explicitly undefined" \
    'function f(a = 5) { console.log(a); } f(undefined);' \
    "5"

expect "a real argument overrides the default, including falsy values" \
    'function f(a = 5) { console.log(a); } f(0);' \
    "0"

expect "a default expression can see earlier parameters" \
    'function f(a, b = a + 1) { console.log(b); } f(10);' \
    "11"

expect "arrow functions get default parameters too" \
    '((a = 3) => console.log(a))();' \
    "3"

echo "deep recursion safety"

# A long left-nested chain used to overflow the C stack in eval()
# rather than fail cleanly - this is what actually crashed Clint on a
# real page (see tus-clint-browser.md). Comfortably past
# JS_TREE_DEPTH_LIMIT, so this must degrade to a clean script error,
# never a crash - the interpreter running standalone here just needs
# to exit 0 with that message, not segfault (which `check`/`expect`
# would otherwise silently miss, since a crashed subshell's $out is
# just empty rather than a visible failure).
deep_chain=$(python3 -c "print('console.log(' + '1' + '+1'*300 + ');')")
expect "a very long expression chain fails cleanly, not with a crash" \
    "$deep_chain" \
    "error: an expression was nested too deeply"

# Comfortably realistic nesting (well under the limit) must still
# evaluate correctly - the guard must not be so tight it breaks
# ordinary code.
shallow_chain=$(python3 -c "print('console.log(' + '1' + '+1'*20 + ');')")
expect "an ordinary, realistically-nested expression still evaluates" \
    "$shallow_chain" \
    "21"

rm -f /tmp/clint-js-test.html
exit $fail

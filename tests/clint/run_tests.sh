#!/bin/sh
#
# Clint's engine, checked on the build host.
#
# Each case is a small document and an assertion about the display
# list it lays out to. Layout is where a browser is wrong in ways that
# still look plausible, so the checks are about positions and sizes
# rather than about "it did not crash".

fail=0

check() {
    if [ "$2" = 0 ]; then
        echo "  [PASS] $1"
    else
        echo "  [FAIL] $1"
        fail=1
    fi
}

# run <html> -> the display list on stdout
run() {
    printf '%s' "$1" | ./dump "${2:-400}"
}

echo "parsing"

out=$(run '<p>hello <b>bold</b> world</p>')
echo "$out" | grep -q '"bold" *$\|bold .*"bold"'
check "an inline element keeps its own style" $?

out=$(run '<p>a<br>b</p>')
first=$(echo "$out" | grep '"a"' | awk '{print $3}')
second=$(echo "$out" | grep '"b"' | awk '{print $3}')
[ -n "$first" ] && [ -n "$second" ] && [ "$first" != "$second" ]
check "<br> starts a new line" $?

out=$(run '<p>x &amp; y &mdash; z</p>')
echo "$out" | grep -q '"&"' && echo "$out" | grep -q '"-"'
check "entities are decoded" $?

out=$(run '<div><script>var a = "<p>not markup</p>";</script>text</div>')
echo "$out" | grep -q '"text"' && ! echo "$out" | grep -q '"markup"'
check "script content is not parsed as markup" $?

out=$(run '<ul><li>one<li>two</ul>')
[ "$(echo "$out" | grep -c '"\*"')" = 2 ]
check "an unclosed <li> is closed by the next one" $?

echo "the cascade"

out=$(run '<ul><li>item</li></ul>')
echo "$out" | grep -q '"\*"'
check "a list item gets a marker" $?

out=$(run '<p style="text-align:right">right</p>' 400)
x=$(echo "$out" | grep '"right"' | awk -F'[ ,]+' '{print $3}')
[ -n "$x" ] && [ "$x" -gt 200 ]
check "text-align:right moves the line to the right edge" $?

out=$(run '<style>p { color: #ff0000 }</style><p>red</p>')
echo "$out" | grep '"red"' | grep -q '#ff0000'
check "a document stylesheet sets colour" $?

out=$(run '<style>p { color: #00ff00 }</style><p style="color:#0000ff">x</p>')
echo "$out" | grep '"x"' | grep -q '#0000ff'
check "the style attribute outranks a stylesheet rule" $?

out=$(run '<style>p { color: #00ff00 !important }</style><p style="color:#0000ff">x</p>')
echo "$out" | grep '"x"' | grep -q '#00ff00'
check "!important outranks the style attribute" $?

out=$(run '<style>.a { color: #112233 } div { color: #445566 }</style><div class="a">x</div>')
echo "$out" | grep '"x"' | grep -q '#112233'
check "a class selector beats a tag selector" $?

out=$(run '<style>main p { color: #abcdef }</style><main><span><p>x</p></span></main>')
echo "$out" | grep '"x"' | grep -q '#abcdef'
check "a descendant selector matches through other elements" $?

out=$(run '<h1>big</h1>')
echo "$out" | grep '"big"' | grep -q 'x3'
check "a heading is larger than body text" $?

out=$(run '<pre>a  b</pre>')
echo "$out" | grep -q '"a  b"'
check "white-space:pre keeps runs of spaces" $?

out=$(run '<p>a  b</p>')
echo "$out" | grep -q '"a"' && echo "$out" | grep -q '"b"'
check "ordinary text collapses whitespace into word breaks" $?

echo "layout"

out=$(run '<p>one two three four five six seven eight nine ten</p>' 200)
lines=$(echo "$out" | grep '"' | awk -F'[ ,]+' '{print $4}' | sort -u | wc -l)
[ "$lines" -gt 1 ]
check "a long paragraph wraps" $?

wide=$(run '<p>one two three four five six seven eight nine ten</p>' 2000 |
        grep '"' | awk -F'[ ,]+' '{print $4}' | sort -u | wc -l)
[ "$wide" = 1 ]
check "the same paragraph fits on one line when there is room" $?

out=$(run '<div style="background:#123456;padding:10px">x</div>')
echo "$out" | grep -q 'rect.*#123456'
check "a background becomes a painted rectangle" $?

out=$(run '<div style="background:#123456">x</div>')
rect_line=$(echo "$out" | grep -n 'rect.*#123456' | head -1 | cut -d: -f1)
text_line=$(echo "$out" | grep -n '"x"' | head -1 | cut -d: -f1)
[ -n "$rect_line" ] && [ -n "$text_line" ] && [ "$rect_line" -lt "$text_line" ]
check "the background is painted before the text on top of it" $?

out=$(run '<a href="/there">link</a>')
echo "$out" | grep -q 'link .*-> /there'
check "a link records a clickable rectangle" $?

out=$(run '<a href="/there">link</a>')
echo "$out" | grep '"link"' | grep -q 'under'
check "a link is underlined" $?

out=$(run '<body><div style="margin:10px 0 30px 20px">a</div><div>b</div></body>')
ax=$(echo "$out" | grep '"a"' | awk -F'[ ,]+' '{print $3}')
ay=$(echo "$out" | grep '"a"' | awk -F'[ ,]+' '{print $4}')
by=$(echo "$out" | grep '"b"' | awk -F'[ ,]+' '{print $4}')
[ "$ax" = 28 ] && [ "$ay" = 18 ] && [ "$by" = 64 ]
check "a four-value margin shorthand sets each side" $?

out=$(run '<body><h1>h</h1><p>p</p></body>')
hy=$(echo "$out" | grep '"h"' | awk -F'[ ,]+' '{print $4}')
[ -n "$hy" ] && [ "$hy" -gt 8 ]
check "the default stylesheet gives a heading its margin" $?

out=$(run '<p style="font-size:1.5em">x</p>')
echo "$out" | grep '"x"' | grep -q 'x2'
check "a fractional em size is rounded after the unit, not before" $?

out=$(run '<style>body{width:60vw}</style><body><p>wide</p></body>' 800)
x=$(echo "$out" | grep 'rect' | head -1 | awk -F'[ x]+' '{print $5}')
[ -n "$x" ] && [ "$x" -gt 400 ]
check "a viewport unit is not mistaken for pixels" $?

out=$(run '<style>body{margin:15vh auto}</style><body><p>x</p></body>' 800)
echo "$out" | grep '"x"' | grep -qv ' 0,'
check "a margin that cannot be resolved leaves the default alone" $?

out=$(run '<p>visible</p><p style="display:none">hidden</p>')
echo "$out" | grep -q '"visible"' && ! echo "$out" | grep -q '"hidden"'
check "display:none removes an element" $?

out=$(run '<head><title>t</title></head><body>body</body>')
echo "$out" | grep -q '"body"' && ! echo "$out" | grep -q '"t"'
check "the head is not rendered" $?

echo "pictures"

# dot.png is 40x20 - the sizing rules are checked against a real
# decode, so a broken PNG decoder fails here rather than at boot.
out=$(run '<p><img src="dot.png" alt="dot"></p>')
echo "$out" | grep -q 'image .* 40x20 *(source 40x20)'
check "a PNG is decoded and drawn at its own size" $?

out=$(run '<p><img src="dot.png" width="80"></p>')
echo "$out" | grep -q 'image .* 80x40'
check "a width attribute scales the height with it" $?

out=$(run '<p><img src="dot.png" width="20" height="20"></p>')
echo "$out" | grep -q 'image .* 20x20'
check "both attributes together are taken as written" $?

out=$(run '<p><img src="dot.png"></p>' 24)
echo "$out" | grep -q 'image .* 24x12'
check "an image wider than the line is scaled down to fit" $?

out=$(run '<p><img src="missing.png" alt="a duck"></p>')
echo "$out" | grep -q '"\[a' && ! echo "$out" | grep -q 'image'
check "an image that cannot be fetched shows its alt text" $?

out=$(run '<p><img src="missing.png" alt=""></p>')
! echo "$out" | grep -q 'text'
check "alt=\"\" means the picture is decoration and is left out" $?

echo "forms"

out=$(run '<form action="/s"><input name="q" size="10" value="hi"></form>')
echo "$out" | grep -q 'field .* 90x26 *text *name=q "hi"'
check "a text input is a box sized by its size attribute" $?

out=$(run '<form><input type="hidden" name="h" value="1"><input name="q"></form>')
[ "$(echo "$out" | grep -c 'field')" = 1 ]
check "a hidden input is not drawn" $?

out=$(run '<form><input type="submit" value="Go"></form>')
echo "$out" | grep -q 'field .* button .* "Go"'
check "a submit button shows its value as its label" $?

out=$(run '<form><button>Send it</button></form>')
echo "$out" | grep -q 'field .* button .* "Send it"'
check "a <button> takes its label from its content" $?

out=$(run '<form><select name="s"><option>one<option selected>two</select></form>')
echo "$out" | grep -q 'field .* select *name=s "two"'
check "a select shows the option marked selected" $?

out=$(run '<form><input type="checkbox" name="c" checked></form>')
[ "$(echo "$out" | grep -c 'rect')" -ge 6 ]
check "a checked checkbox is drawn with its mark" $?

out=$(run '<p style="text-align:center"><input name="q" size="4"></p>' 400)
fx=$(echo "$out" | grep 'field' | awk -F'[ ,]+' '{print $3}')
rx=$(echo "$out" | grep 'rect' | head -1 | awk -F'[ ,]+' '{print $3}')
[ -n "$fx" ] && [ "$fx" = "$rx" ]
check "a centred control moves with the box that was drawn for it" $?

echo "inline-block"

out=$(run '<div><span style="display:inline-block;background:#eeeeee">AA</span><span style="display:inline-block;background:#dddddd">BB</span></div>' 400)
[ "$(echo "$out" | grep -c 'rect .* 16x16')" = 2 ]
check "an inline-block shrinks to fit its contents" $?

second=$(echo "$out" | grep '"BB"' | awk -F'[ ,]+' '{print $3}')
[ "$second" = 16 ]
check "two inline-blocks sit next to each other" $?

out=$(run '<style>.d{display:inline-box;display:inline-block}</style><div><span class="d"><span style="display:block;background:#eeeeee">AA</span></span></div>' 400)
echo "$out" | grep -q 'rect .* 16x16'
check "the last of two declarations of a property wins" $?

echo "text"

out=$(run '<p>G&#246;rseller</p>')
echo "$out" | grep -q '72x16'
check "an accented letter is one cell wide, not two bytes" $?

out=$(run '<p>&ccedil;a&#287;</p>')
echo "$out" | grep -q '"çağ"'
check "named and numeric entities become the letters themselves" $?

out=$(printf '<p>t\375klay\375n</p>' | ./dump 400 -c iso-8859-9)
echo "$out" | grep -q '"tıklayın"'
check "an ISO-8859-9 page is read as Turkish, not as Latin-1" $?

out=$(printf '<p>\223quoted\224</p>' | ./dump 400 -c iso-8859-1)
echo "$out" | grep -q '"quoted"'
check "a Latin-1 page uses the windows-1252 quotes every page means" $?

out=$(run '<p>&#9731;</p>')
echo "$out" | grep -q '"?"'
check "a character with no glyph becomes one that has" $?

exit $fail

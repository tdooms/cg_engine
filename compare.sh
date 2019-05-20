#!/bin/bash

for filename in $1/*.png; do
name=${filename: 0: -4}
echo $name;
w=$(identify -format %w $filename);
h=$(identify -format %h $filename);
total=$(( w * h ));
diff=$(compare -metric AE -fuzz 10% $name".png" $name".bmp" /tmp/TEMP.png 2>&1);
answer=$(bc <<< "scale=10;$diff/$total*100");
echo $answer;
if (( $(echo "$answer > $2" |bc -l) )); then
	sxiv /tmp/TEMP.png $name".png" $name".bmp";
fi
done

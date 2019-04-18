#/usr/bin sh

#cat urls3.txt | ./mark2 c 2 > model5.txt

URL="file://"`pwd`

awk -v a="$URL/" '{print a$0}' urls_00.txt | ./mark2 c 2 > model_00.txt
cat urls_01.txt | ./mark2 c 2 > model_01.txt
awk -v a="$URL/" '{print a$0}' urls_02.txt | ./mark2 c 2 > model_02.txt
awk -v a="$URL/" '{print a$0}' urls_03.txt | ./mark2 c 2 > model_03.txt


for i in `find . -name '*.png'`; do
    fl=`echo $i | cut -d '/' -f 2`
    fl=`echo $fl | cut -d '.' -f 1`
    convert $i "$fl.jpg"
done


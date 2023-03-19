for s in {0..9};
do ./pathsim -p 500k.txt -i settings.txt -o data/calibration/1pcttest.$s -d $s 
done;

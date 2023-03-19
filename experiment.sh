pathName=$1
for s in {0..9};
do ./pathSim -p 500k.txt -i samplePaths/$pathName.txt -o data/rawdata/$pathName.$s -d $s;
echo Trial $s Finished;
done;
./analysis data/rawdata/$pathName > data/summary/$pathName.summary;
python graphing.py data/rawdata/$pathName;
open graphs/$pathName.png;

cd multitree-kautz/
./make-scheduletable.sh

cd ..
cd platform-generator/
python3 kautz.py --degree 16
python3 kautz.py --degree 32
python3 kautz.py --degree 64
cd ..

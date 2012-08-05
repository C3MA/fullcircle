#/bin/bash
# requires mktemp and imagemagick/ffmpeg!

if [ "$1" == "" ]; then
	echo "Path to .fcs-file required"
	exit 0
fi
TEMP=$(mktemp -d)

FILENAME=$(basename $1 .fcs)

FRAMERATE_IN=6

FRAMERATE_OUT=24

echo "Temporary folder:" $TEMP

echo "Converting sequence to images..."
./fc-2gif -s $1 -t $TEMP -w 320 -h 240 >/dev/null

echo "Converting image sequence to animation (this might take some time)"

#create a gif -- takes a looooong time
#time convert -delay 10 -loop 0 $TEMP/*.png $2

#create an mp4 avi
echo "creating mp4 file"
ffmpeg -y -r $FRAMERATE_IN -f image2 -i $TEMP/%5d.png -r $FRAMERATE_OUT $TEMP/$FILENAME.mp4 2>/dev/null
#create an webm file
echo "creating webm file"
ffmpeg -y -r $FRAMERATE_IN -f image2 -i $TEMP/%5d.png -r $FRAMERATE_OUT $TEMP/$FILENAME.webm 2>/dev/null
#create an ogv file
echo "creating ogv file"
ffmpeg -y -r $FRAMERATE_IN -f image2 -i $TEMP/%5d.png -r $FRAMERATE_OUT $TEMP/$FILENAME.ogv 2>/dev/null
echo "copying first frame for preview"
cp $TEMP/00000.png $TEMP/$FILENAME.png

echo "uploading files to webspace"
scp $TEMP/$FILENAME.{mp4,webm,ogv,png} fullcircle@comsound.de:/srv/fullcircle/data/

rm -rf $TEMP

echo "Done!"

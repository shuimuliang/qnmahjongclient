#! /bin/bash

CURRENT_DIR=`dirname $0`

# input paths
IMAGE_DIR=$CURRENT_DIR/../Resources/images
FONT_DIR=$CURRENT_DIR/../Resources/font
TEMP_IMAGE_DIR=$IMAGE_DIR/../temp

# path that game proj use
GAME_IMAGE_PATH=$CURRENT_DIR/../casino/Resources/
GAME_FONT_PATH=$CURRENT_DIR/../casino/Resources/font

# temporary path to place the sprite sheets
OUTPUT_PATH=$CURRENT_DIR/../spriteSheets
OUTPUT_PATH_PVR=$OUTPUT_PATH/packagedPVR
OUTPUT_PATH_PNG=$OUTPUT_PATH/packagedPNG


# path of the texture packer command line tool
TP=/usr/local/bin/TexturePacker

# $1: Source Directory where the assets are located
# $2: Output File Name without extension
# $3: RGB Quality factor
# $4: Scale factor
# $5: Max-Size factor
# $6: Texture Type (PNG, PVR.CCZ)
# $7: Texture format
pack_textures() {

${TP} --smart-update \
--texture-format $7 \
--format cocos2d \
--data "$2".plist \
--sheet "$2".$6 \
--maxrects-heuristics best \
--scale $4 \
--shape-padding 1 \
--max-size $5 \
--opt "$3" \
--trim-mode None \
--premultiply-alpha \
--enable-rotation \
--size-constraints AnySize \
$1/

}
#--extrude 2 \

#--enable-rotation \
#--premultiply-alpha #\#


# check the output path
if [ -d $TEMP_IMAGE_DIR ];then
:
else
mkdir $TEMP_IMAGE_DIR
fi

if [ -d $OUTPUT_PATH ];then
:
else
mkdir $OUTPUT_PATH
fi

if [ -d $OUTPUT_PATH_PVR ]
then
:
else
mkdir $OUTPUT_PATH_PVR
fi

if [ -d $OUTPUT_PATH_PNG ]
then
:
else
mkdir $OUTPUT_PATH_PNG
fi

#copy temp resources
# cp -rf $IMAGE_DIR/* $TEMP_IMAGE_DIR/
rsync --exclude=.svn $IMAGE_DIR/ $TEMP_IMAGE_DIR/ -r

echo "copy local file..."
./UpdateLocalFIle $IMAGE_DIR/../hn/images $TEMP_IMAGE_DIR/

for i in $TEMP_IMAGE_DIR/*
do
if [ -d $i ]
then
    for j in $i/*
    do
    if [ -d $j ]
    then
        # if [ `basename $j` != ".svn" ]
        # then
            for k in `find $j`
            do
            if [ -d $k ]
            then
            :
            else
            #        echo $k
            mv $k $i
            fi
            done
        # fi
    fi
    done
fi
done

echo "cleaning  temp file..."
rm -f $OUTPUT_PATH_PVR/*.pvr.ccz
rm -f $OUTPUT_PATH_PVR/*.plist
rm -f $OUTPUT_PATH_PVR/*.jpg
rm -f $OUTPUT_PATH_PVR/*.png

echo "building..."

# do the job
for i in $TEMP_IMAGE_DIR/*
do
if [ -d $i ]
then
    if [ `basename $i` != "images_local" ]
    then
    spriteSheetName=`basename $i`
    #pack_textures $i $OUTPUT_PATH_PNG/$spriteSheetName 'RGBA8888' 1 2048 'png' "png"
    pack_textures $i $OUTPUT_PATH_PVR/$spriteSheetName 'RGBA8888' 1 2048 'pvr.ccz' "pvr2ccz"
    fi
fi
done

# cp them to the game proj image path
echo "cleaning..."
rm -f $GAME_IMAGE_PATH/*.pvr.ccz
rm -f $GAME_IMAGE_PATH/*.plist
rm -f $GAME_IMAGE_PATH/*.jpg
rm -f $GAME_IMAGE_PATH/*.png
rm -f $GAME_FONT_PATH/*.png
rm -f $GAME_FONT_PATH/*.fnt

echo "copy..."
cp -f $OUTPUT_PATH_PVR/* $GAME_IMAGE_PATH
cp -f $FONT_DIR/*.png $GAME_FONT_PATH
cp -f $FONT_DIR/*.fnt $GAME_FONT_PATH
cp -f $TEMP_IMAGE_DIR/*.jpg $GAME_IMAGE_PATH
#cp -f $TEMP_IMAGE_DIR/images_local/*.jpg $GAME_IMAGE_PATH

#remove temp files
rm -rf $TEMP_IMAGE_DIR
rm -f $OUTPUT_PATH_PVR/*
echo "packTextures cocosbuilder finished"

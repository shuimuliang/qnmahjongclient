#! /bin/bash

CURRENT_DIR=`dirname $0`

# path that game proj use
GAME_IMAGE_PATH=$CURRENT_DIR/../casino/Resources/
GAME_SCENE_PATH=$CURRENT_DIR/../casino/Scenes/


./updateCCB $GAME_IMAGE_PATH $GAME_SCENE_PATH

echo "finished"
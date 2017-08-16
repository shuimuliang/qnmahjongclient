#! /bin/bash
sh _pack_cocosbuilder_hn.sh
sh _pack_release_hn.sh $1
sh updateCCB.sh
echo "finished"

python ../../cocos2d-x-2.2.6/tools/cocos2d-console/console/cocos2d.py luacompile -s ../../cocos2d-x-2.2.6/projects/Casino/Resources/db -d ../../cocos2d-x-2.2.6/projects/Casino/Resources/dbTemp --disable-compile -e true -k 6a8083f8-8730-11e4-8627-7831c1c2f6e6 -b WANNA_DECRYPTION_??

python ../../cocos2d-x-2.2.6/tools/cocos2d-console/console/cocos2d.py luacompile -s ../../cocos2d-x-2.2.6/projects/Casino/Resources/lua -d ../../cocos2d-x-2.2.6/projects/Casino/Resources/luaTemp --disable-compile -e true -k 6a8083f8-8730-11e4-8627-7831c1c2f6e6 -b WANNA_DECRYPTION_??

cd ../../cocos2d-x-2.2.6/projects/Casino/Resources

cp -f -r luaTemp/ lua/
cp -f -r dbTemp/ db/

rm -rf luaTemp
rm -rf dbTemp

echo "lua build finish!"

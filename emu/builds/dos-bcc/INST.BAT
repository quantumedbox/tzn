IF %INSTFLD% == "" GOTO ERR
mkdir %INSTFLD%
imgmount a DISK01.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK02.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK03.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK04.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK05.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK06.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK07.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
imgmount a DISK08.IMG -size 512,63,16,20
copy A:\*.* %INSTFLD%
imgmount -u a
GOTO FIN
:ERR
echo "Set INSTFLD env variable to directory to which file should be copied"
:FIN
echo "Done"

#!/bin/bash
#
# setup envirnment variable

source notice_build.sh

#create install directories
if [ ! -d $NKHOME/bin ]; then mkdir $NKHOME/bin; fi
if [ ! -d $NKHOME/include ]; then mkdir $NKHOME/include; fi
if [ ! -d $NKHOME/lib ]; then mkdir $NKHOME/lib; fi

CORE_USB2="linux_interface 6uvme"
CORE_USB3="nkusb nkusbroot nk6uvme nk6uvmeroot nkadc nkadcroot usb3com usb3com_root"

PACKAGE_USB2="fadtdc ngtfadc pdrs apgcd gcounter nbeam100 ngttest apgcd_hist jousb nfadc100 pfadc400 nfadc25 png camac nfadc400 relay display kifadc nfadc500 sadc fadc100 nfadc64 fadc200 kstar ngt400 tdc64m fadc400 ngt4400 tdc64n fadc500 kstrig ngt44008G ntcp V792 fadc64"
PACKAGE_USB3="kstarusb3 kstrigusb3 knutrig kfadc kfadc_root kfadc500 kfadc64 nkprogrammer nfadc100ux fadtdcux"
PACKAGE_PROGRAMMER="fadtdc kfadc64 kstar nbeam100 nfadc100 nfadc400 nfadc64 tdc64n kfadc500" #pdrs left out


echo -e "\n\n\n---------------------------------- processing USB2 DRIVER ----------------------------------\n\n\n"
cd $NKHOME/driver

FILE=Makefile
if [ -f $FILE ]; then
   echo " processing $FILE"
   make -f $FILE clean
   if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
   make -f $FILE 
   if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
fi

FILE=install.sh
if [ -f $FILE ]; then
   echo " processing $FILE"
   ./$FILE
   if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
fi

echo -e "\n\n\n---------------------------------- processing USB2 CORE ----------------------------------\n\n\n"
cd $NKHOME/src

for X in $CORE_USB2;   
do
   echo "--- processing $X ----------------------------------"
   cd $X
   FILE="Makefile_c Makefile_cpp Makefile_root Makefile_org_c Makefile_org_cpp Makefile_org_root Makefile_mod_c Makefile_mod_cpp Makefile_mod_root"
   for Y in $FILE;
   do   
      if [ -f $Y ]; then
         echo " processing $Y"
         make -f $Y clean
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y install 
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
      fi
   done

   cd .. 
done

echo -e "\n\n\n---------------------------------- processing USB3 CORE ----------------------------------\n\n\n"
cd $NKHOME/src

for X in $CORE_USB3;   
do
   echo "--- processing $X ----------------------------------"
   cd $X
   FILE="Makefile"
   for Y in $FILE;
   do   
      if [ -f $Y ]; then
         echo " processing $FILE"
         make -f $Y clean
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y install 
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
      fi
   done

   cd .. 
done

echo -e "\n\n\n---------------------------------- processing USB2 PACKAGES ----------------------------------\n\n\n"
cd $NKHOME/src

for X in $PACKAGE_USB2;   
do
   echo "--- processing $X ----------------------------------"
   cd $X
   FILE="Makefile_c Makefile_cpp Makefile_root Makefile_org_c Makefile_org_cpp Makefile_org_root Makefile_mod_c Makefile_mod_cpp Makefile_mod_root"
   for Y in $FILE;
   do   
      if [ -f $Y ]; then
         echo " processing $Y"
         make -f $Y clean
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y install 
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
      fi
   done

   cd .. 
done

echo -e "\n\n\n---------------------------------- processing USB3 PACKAGES ----------------------------------\n\n\n"
cd $NKHOME/src

for X in $PACKAGE_USB3;   
do
   echo "--- processing $X ----------------------------------"
   cd $X
   FILE="Makefile"
   for Y in $FILE;
   do   
      if [ -f $Y ]; then
         echo " processing $Y"
         make -f $Y clean
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y install 
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
      fi
   done

   PACKAGE_USB3_SUB=`ls` 
   for Y in $PACKAGE_USB3_SUB;
   do
      if [ -d $Y ]; then
         echo "------ processing $Y ----------------------------------"
         cd $Y
         FILE="Makefile"
         for Z in $FILE;
         do   
            if [ -f $Z ]; then
               echo " processing $Z"
               make -f $Z clean
               if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
               make -f $Z
               if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
               make -f $Z install 
               if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
            fi
         done
         cd .. 
      fi
   done
   cd .. 
done

#------------------------------------------------------
# formware upgrade directory compilation

echo -e "\n\n\n---------------------------------- processing programmer ----------------------------------\n\n\n"
cd $NKHOME/programmer

for X in $PACKAGE_PROGRAMMER;
do   
   echo "--- processing $X ----------------------------------"
   cd $X
   FILE="Makefile_update Makefile_TLT Makefile"
   for Y in $FILE;
   do
      if [ -f $Y ]; then
         echo " processing $Y"
         make -f $Y clean
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
         make -f $Y
         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
#         make -f $Y install 
#         if [ $? != 0 ]; then cd $NKHOME; exit 1; fi
      fi
   done
   cd .. 
done

exit

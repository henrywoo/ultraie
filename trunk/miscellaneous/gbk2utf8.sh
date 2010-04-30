#!/bin/sh 
# Author: wufuhemg@gmail.com
# Shell script to find out all the files under a directory and 
# its subdirectories. This also takes into consideration those files
# or directories which have spaces or newlines in their names 

DIR="."
pattern1="*.txt";
pattern2="*.html";
pattern3="*.php";
pattern4="*.css";
pattern5="*.htm";
pattern6="*.xml";
pattern7="*.js";

function list_files()
{
  if !(test -d "$1")  
    then echo $1; return;
  fi

    cd "$1"
    ###echo; echo `pwd`:; #Display Directory name

    for i in *
      do
        if test -d "$i"  #if dictionary
          then  
            list_files "$i" #recursively list files
            cd ..
        else
          CURPATH=`pwd`;
      case $i in
        $pattern1)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern2)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern3)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern4)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern5)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern6)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        case $i in
        $pattern7)
        `iconv -f gbk -t utf-8 $CURPATH/$i >$CURPATH/$i.bak`;;
      esac

        `mv -f $CURPATH/$i.bak $CURPATH/$i`;


      fi

        done
}

if [ $# -eq 0 ]
then list_files .
exit 0
fi

for i in $*
do
DIR="$1"
list_files "$DIR"
shift 1 #To read next directory/file name
done

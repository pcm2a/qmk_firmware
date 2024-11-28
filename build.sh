 #1140  qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a
 #1141  qmk compile keychron/c3_pro/ansi/red -km keychron
 #1155  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1156  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1159  qmk compile -kb converter/usb_usb/hasu -km pcm2a
 #1161  qmk compile -kb nullbitsco/holly -km pcm2a
 # make spiderisland/split78:pcm2a:flash
#!/bin/bash

function compile() {
  qmk compile -kb $1 -km $2
  mkdir build
  mv *.bin build/
}

function compile_sonix() {
  mkdir build
  cd ../qmk_sonix
  make keychron/k8/rgb/ansi:pcm2a
  mv *.bin ../qmk_firmware/build/ 
}

function compile_spider() {
  make spiderisland/split78:pcm2a
  mkdir build
  mv *.hex build/
}

function flash_spider() {
  make spiderisland/split78:pcm2a:flash
  mkdir build
  mv *.hex build/  
}

function figure_it_out() {
  case $1 in
    "1")
      compile "gmmk/gmmk2/p96/ansi" "pcm2a"
      break;;
    "2")
      compile "keychron/c3_pro/ansi/red" "pcm2a"
      break;;
    "3")
      compile "converter/usb_usb/hasu" "pcm2a"
      break;;
    "4")
      compile "nullbitsco/holly" "pcm2a"
      break;;
    "5")
      compile_sonix
      break;;
    "6")
      compile_spider
      break;;
    "7")
      flash_spider
      break;;
  *)
  echo "Ooops";;
  esac
}

if [ -n "$1" ]
then
  figure_it_out $1
else
  PS3="Choose: "
  select kb in Gmmk C3_Pro Hasu Holly K8 Spiderisland_build Spiderisland_flash
  do
    case $kb in
      "Gmmk")
        figure_it_out "1"
        break;;
      "C3_Pro")
        figure_it_out "2"
        break;;
      "Hasu")
        figure_it_out "3"
        break;;
      "Holly")
        figure_it_out "4"
        break;;
      "K8")
        figure_it_out "5"
        break;;
      "Spiderisland_build")
        figure_it_out "6"
        break;;
      "Spiderisland_flash")
        figure_it_out "7"
        break;;
      "Quit")
         echo "We're done"
         break;;
      *)
      echo "Ooops";;
    esac
  done
fi

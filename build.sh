 #1140  qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a
 #1141  qmk compile keychron/c3_pro/ansi/red -km keychron
 #1155  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1156  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1159  qmk compile -kb converter/usb_usb/hasu -km pcm2a
 #1161  qmk compile -kb nullbitsco/holly -km pcm2a
#!/bin/bash

if [ $1 != '' ]
then
    case $1 in
        "1")
            qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a
            break;;
        "2")
            qmk compile -kb keychron/c3_pro/ansi/red -km pcm2a
            break;;
        "3")
            qmk compile -kb converter/usb_usb/hasu -km pcm2a
            break;;
        "4")
            qmk compile -kb nullbitsco/holly -km pcm2a
            break;;
        "6")
            cd /android/workspace/qmk/qmk_sonix
            make keychron/k8/rgb/ansi:pcm2a
            break;;
        "7")
           echo "We're done"
           break;;
        *)
           echo "Ooops";;
    esac
else
  PS3="Choose: "
  select kb in Gmmk C3_Pro K8 Hasu Holly
  do
    case $kb in
        "Gmmk")
            qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a
            break;;
        "C3_Pro")
            qmk compile -kb keychron/c3_pro/ansi/red -km pcm2a
            break;;
        "Hasu")
            qmk compile -kb converter/usb_usb/hasu -km pcm2a
            break;;
        "Holly")
            qmk compile -kb nullbitsco/holly -km pcm2a
            break;;
        "K8")
            cd /android/workspace/qmk/qmk_sonix
            make keychron/k8/rgb/ansi:pcm2a
            break;;
        "Quit")
           echo "We're done"
           break;;
        *)
           echo "Ooops";;
    esac
  done
fi

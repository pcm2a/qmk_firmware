 #1140  qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a
 #1141  qmk compile keychron/c3_pro/ansi/red -km keychron
 #1155  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1156  qmk compile -kb keychron/c3_pro/ansi/red -km keychron
 #1159  qmk compile -kb converter/usb_usb/hasu -km pcm2a
 #1161  qmk compile -kb nullbitsco/holly -km pcm2a
#!/bin/bash

PS3="Choose:"

select kb in Gmmk C3_Pro K8 Hasu Holly
do
    case $kb in
        "Gmmk")
            qmk compile -kb gmmk/gmmk2/p96/ansi -km pcm2a;;
        "C3_Pro")
            qmk compile -kb keychron/c3_pro/ansi/red -km pcm2a;;
        "Hasu")
            qmk compile -kb converter/usb_usb/hasu -km pcm2a;;
        "Holly")
            qmk compile -kb nullbitsco/holly -km pcm2a;;
        "K8")
            echo "Oops, no K8 support yet";;
        "Quit")
           echo "We're done"
           break;;
        *)
           echo "Ooops";;
    esac
done

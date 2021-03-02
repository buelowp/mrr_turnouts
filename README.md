# QT5 Turnout control using MQTT and a Raspberry Pi

This project aims to use a Pi and those 8 turnout 5V control boards which interface nicely with the PI.
Theoretically, this can control as many turnouts as there are GPIO available (~28, depending). It does
not allow for watching the aux outputs for indications, there are not enough GPIO to do that efficiently.

I chose the Pi because it allows me to plug in using ethernet, which is nice, my MRR is going to have a
lot of controllers doing things, and if all WiFi, I may overload my local WiFi AP. Being on ethernet means
I don't have to worry about that, and I don't have to worry about connectivity issues. Well, I shouldn't
anyway.

It's also cheap and everyone who sells add on boards probably also indicates compatibility with the Pi, so
you can tell if what you're getting is easy to integrate. See below for the 8 port relay boards I plan
to use.

My MRR has 22 switches, a mix of #6 left and right, and some wyes as well. The goal here is to allow
me to toggle them individually using MQTT. I want a control board for my N scale 4x8 which is a touch
screen on a Pi 4 which can tell me where each train is (using a Pi based current monitor for zones)
and, let me make switch decisions manually, but also allow for some remote control. Basically, start
a few trains, let them run their routes, and do some switch operations while the auto trains stop
nicely when the signals tell them to, if the route is blocked.

More to come, this is just starting, but the code works now, and does what I wanted. Next up, get
the current monitor for track segments up and running, and then an interface into JMRI so I can
tell it to stop and start trains as needed. I may use DCC++ for this too, but it's a bigger layout,
and I still haven't figured out how to do the boosters for it to get power to all the districts.

##

HW used

* RPi 3 or 4, you choose. This code isn't very taxing. It's currently tested on a Pi 3.
* https://www.amazon.com/gp/product/B00KTELP3I/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1

That's it. Plug it into the network however you want. See the wiki for a MQTT JSON description
of the messaging


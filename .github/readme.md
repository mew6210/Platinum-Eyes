# Platinum-Eyes
Overlay app for Warframe. Provides a quick, realtime lookup of prices for the items you dropped from Warframe Market.


## ⭐Features
- [x] ***Fissure rewards***
 

![Fissure rewards screenshot](relicRewards1.png "1")

![Fissure rewards screenshot](relicRewards2.png "2")

&nbsp;
&nbsp;
&nbsp;
###
###
- [x] ***Relic possible rewards***


![Fissure rewards screenshot](relicTitleScreenshot1.png "3")

![Fissure rewards screenshot](relicTitleScreenshot2.png "4")

- [ ] Serial relic scanning

- [ ] Inventory scanning
- [ ] Warframe-Market integration


## ❗ Is it bannable?
All this app does, is something that you can do yourself, but much faster. You absolutely could search up in warframe market all the 4 items that you dropped in your fissure run, but why bother. Even if, the app takes screenshot of your screen which is completely not invasive. This app ***doesn't*** modify warframe files, so everything should be good. Remember though, it is still against Warframe TOS.

## ❓ How does it work?
Platinum Eyes can be split into 4 parts.
- ### 📸 Screenshot
     The app takes a screenshot of your display, in a specific place where item(or relic) names are.\
     It is platform specific, for windows it uses winapi, and for linux it will be using x11.
     
- ### 🏭 Ocr
     Image is then processed by ocr, to determine what words are in the picture.\
     They are also easily the heaviest part of the app, tesseract's training data alone is 20mb.
     
- ### 🚚 Fetch data
     After we get the data from image, for example an item name, we can query warframe-market for the avalible orders for that given item.\
     This is fairly well optimized.
     
- ### 🖥️ Display
     Lastly, we display the data that we recieve from warframe-market on the screen for a user.\
     **This app uses a weird mix of sfml and imgui.**


## 🖥️ Supported Resolutions
[Looking back here,](#-screenshot) it begs a question. How do we know WHERE to take a screenshot, or what to take a screenshot of? **Well, we can only guess.** Since resolutions vary, the places for where to take a screenshot also varies.\
How ui scaling in warframe works is currently beyond me. There is a formula that works in maybe 80% of the cases, so it is pretty fine for most resolutions, but not all. I can guarantee, that for those resolutions everything works well:
- 1920x1080
- 3440x1440
- 1366x768

But for the others, well u can just pray that it will work. Saying that, i am intending on bettering the formula so it works on all the resolutions. In the future there will be a way to tell the program where to take a screenshot, so that even if you have some very obscure resolutions, you will be able to set it up so it works just fine. So, if it doesn't work for your resolution, please open an issue and i will be more than happy to help.
  
## 💻 Supported Operating Systems

- ### 🟢 Windows
  It is primarly targeted for windows, everything will work fine here.

- ### 🟡 Linux
  I am intending on making this work on Linux too, using x11 instead of winapi. But that is in the future.
- ### 🔴 MacOs
  No.

- 
## 🔧 How to build

Todo...


        

<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/fabio-rcs/CYCLOGARD">
    <img src="Docs/logo.png" alt="Logo" width="150" height="100">
  </a>

<h3 align="center">CYCLOGARD</h3>

  <p align="center">
    Consists on the dectection of vehicles that doesn't respect the minimum safety distance from cyclists.
    <br />
    <a href="https://github.com/fabio-rcs/CYCLOGARD/issues">Report Bug</a>
    ·
    <a href="https://github.com/fabio-rcs/CYCLOGARD/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
<div align="center">
<img  src="Docs/logo.png" alt="Logo" width="400">
</div>
For the class Automation Systems Project, we were challenged to create a system to help protect cyclists: a device that detects incoming cars and alerts the cyclist of a possible danger, is also able to alert the vehicle driver with a sound or light warning, as well as to use the smartphone through an app to take a picture of the license plate number of the car, when the driver doesn't respect the minimum safety distance. All the data is store using an SQL server.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ### Built With

* [![Next][Next.js]][Next-url]
* [![React][React.js]][React-url]
* [![Vue][Vue.js]][Vue-url]
* [![Angular][Angular.io]][Angular-url]
* [![Svelte][Svelte.dev]][Svelte-url]
* [![Laravel][Laravel.com]][Laravel-url]
* [![Bootstrap][Bootstrap.com]][Bootstrap-url]
* [![JQuery][JQuery.com]][JQuery-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->



<!-- GETTING STARTED -->
## Getting Started



### Prerequisites
You need to install [Arduino IDE](https://www.arduino.cc/en/software) or other IDE and install the [New Ping](https://www.arduino.cc/reference/en/libraries/newping/) library. 

### Installation
To install the project, clone the repository using a GitHub manager or by running the following lines:
```
git clone https://github.com/fabio-rcs/CYCLOGARD.git
```

<!-- USAGE EXAMPLES -->
## Usage
To use, you can install the apk of the android app and upload the `.ino` code to your microcontroller. There are a lot of versions of this code, but the final version is the [BLE_updatable](https://github.com/fabio-rcs/CYCLOGARD/blob/main/ESP/ESP_BLE_updatable/ESP_BLE_updatable.ino). 
The bluetooth specifications of the ESP32 as a bluetooth device are:

Name: "CYCLOGARD"
Service UUID: "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

Distance_1, distance_2 and the bool will be sent through here in the format "111;222;b"

CHARACTERISTIC_UUID_TX: "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

Commands will be received through here:

CHARACTERISTIC_UUID_RX: "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

It's supposed to be the boolean variable that tells the android app to take a picture. This boolean will have the value "1" when there's a car in the threshold distance and "0" otherwise.
It's possible to activate or deactivate the LED and the horn with the following commands via bluetooth:

Leter "A" activates the LED;

Letter "B" deactivates the LED;

Letter "C" activates the horn;

Letter "D" deactivates the horn.


The database has the following characteristics:

Table name: 	CYCLOGARD

Fields:		

    id  -> id is automatic
    datetime  -> send as yyyy-MM-dd hh:mm:ss
    gps  -> localization to be sent
    link  -> link to the picture 
    distance  -> mean or minimum distance of the ocurrence
			

<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Fábio Sousa - fabiorsousa81@ua.pt

João Valinho - joao.valinho@ua.pt

Nuno Pereira - nafonsofp@ua.pt

Project Link: [CYCLOGARD](https://github.com/fabio-rcs/CYCLOGARD)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* Professor Vítor Santos - vitor@ua.pt

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/fabio-rcs/CYCLOGARD.svg?style=for-the-badge
[contributors-url]: https://github.com/fabio-rcs/CYCLOGARD/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/fabio-rcs/CYCLOGARD.svg?style=for-the-badge
[forks-url]: https://github.com/fabio-rcs/CYCLOGARD/network/members
[stars-shield]: https://img.shields.io/github/stars/fabio-rcs/CYCLOGARD.svg?style=for-the-badge
[stars-url]: https://github.com/fabio-rcs/CYCLOGARD/stargazers
[issues-shield]: https://img.shields.io/github/issues/fabio-rcs/CYCLOGARD.svg?style=for-the-badge
[issues-url]: https://github.com/fabio-rcs/CYCLOGARD/issues
[license-shield]: https://img.shields.io/github/license/fabio-rcs/CYCLOGARD.svg?style=for-the-badge
[license-url]: https://github.com/fabio-rcs/CYCLOGARD/blob/main/LICENSE
[product-screenshot]: Docs/logo.png


<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
[![SuperNimbusWebsite][supernimbus-website-shield]][supernimbus-website-url]

# Hosting dedicated multiplayer servers using UE5 and GameLift

<!-- ABOUT THE PROJECT -->
## About The Project

![Client Screeshot][client-screenshot]


This is a sample project that implements a basic multiplayer HubWorld in Unreal Engine 5.2. It provides a basic client implementation using BrainCloud, as well as a dedicated server that can be hosted on GameLift. It is the end product of a tutorial series that covers building UE5 from source, building the GameLift Server SDK and building, hosting and debugging a basic multiplayer HubWorld on GameLift and BrainCloud.

You can find the tutorial here: >TutorialLink<



### Built With

* [![UnrealEngine][unrealengine-shield]][unrealengine-url]
* [![GameLift][gamelift-shield]][gamelift-url]
* [![BrainCloud][braincloud-shield]][braincloud-url]

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple example steps.

### Prerequisites

#### Software
To use this sample project, you'll need the following things installed:
* Unreal Engine 5.2 built from source
* Visual Studio Community 2019
  
Optional:
* AWS CLI (To test your dedicated server with a GameLift Anywhere Fleet)
* CMAKE (To build the GameLift server SDK with different settings)

#### Accounts
Additionally you will need the following things configured. Everything below is described in details in the Tutorial series found here: >TutorialLink<

AWS account configured with:
* An EC2 GameLift fleet
* A GameLift queue that includes the fleet as a location
* A IAM user for the BrainCloud integration

BrainCloud account and project configured with:
* GameLift Integration
* A custom server
* A hub world lobby

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/samuelhegner-supernimbus/GameLiftTutorial.git
   ```
2. Initialise the Git submodules
   ```sh
   git submodule init
   ```
3. Pull the BrainCloud Client plugin
   ```sh
   git submodule update
   ```
4. Generate the projects VisualStudio solution file

![Usage Screenshot][usage-screenshot]



## Usage

If you have all of the account prerequisites set up and you just want to hook up your BrainCloud environment mage sure you change the following variables in the GameInstance blueprint:

* AppId: The app id of your BrainCloud app
* SecretKey: The secret key of you BrainCloud app
* LobbyName: The name you gave your BrainCloud lobby

![Usage Screenshot][usage-screenshot]

Afterwards, you should be able to launch the client in the editor or from a packaged client build. If configured correctly, you'll join a GameLift hosted server.

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

## Contact

Email us: contact@supernimbus.net

Project Link: [https://github.com/samuelhegner-supernimbus/GameLiftTutorial](https://github.com/samuelhegner-supernimbus/GameLiftTutorial)



## Resources

* 

<!-- License Links -->
[license-shield]: https://img.shields.io/github/license/samuelhegner-supernimbus/GameLiftTutorial.svg?style=for-the-badge
[license-url]: https://github.com/samuelhegner-supernimbus/GameLiftTutorial/blob/main/LICENSE.txt

<!-- SuperNimbus Linked In Links -->
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=0072b1
[linkedin-url]: https://www.linkedin.com/company/supernimbus-ltd/

<!-- SuperNimbus Website Links -->
[supernimbus-website-shield]: https://img.shields.io/badge/-SuperNimbus-black.svg?style=for-the-badge&logo=data:image/png;base64,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&colorB=3e0c4e
[supernimbus-website-url]: https://supernimbus.net/

<!-- UE5 Links -->
[unrealengine-shield]: https://img.shields.io/badge/-Unreal_Engine-black?style=plastic&logo=unrealengine&color=0d0f26
[unrealengine-url]: https://www.unrealengine.com/en-US/unreal-engine-5

<!-- GameLift Links -->
[gamelift-shield]: https://img.shields.io/badge/-GameLift-black.svg?style=plastic&logo=data:image/png;base64,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&color=FF9900
[gamelift-url]: https://aws.amazon.com/gamelift/

<!-- BrainCloud Links -->
[braincloud-shield]: https://img.shields.io/badge/-BrainCloud-black.svg?style=plastic&logo=data:image/png;base64,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&color=009fff
[braincloud-url]: https://getbraincloud.com/

[client-screenshot]: images/project-screeshot.png

[usage-screenshot]: images/usage-screenshot.png

[generate-solution-screenshot]: images/generate-solution-screenshot.png

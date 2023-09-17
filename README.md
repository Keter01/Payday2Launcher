# Payday 2 Launcher

This is a launcher for Payday 2 that allows you to manage mods profiles. This launcher is made for Marcmb's ECM rushers, but a more general version is planned. For now, the 2 checkboxes have no effect, but they will be added in the future (soon(one day(maybe))).

## Installation

Download the zip file from the releases page and extract it to your Payday 2 folder. Then add the following line to your launch options in Steam:
```bash
"C:\Program Files (x86)\Steam\steamapps\common\Payday 2\Payday2Launcher\launch.bat" %command%
```
If you installed steam in a different folder, change the path accordingly.
This will allow you to launch the launcher from Steam instead of the game itself.

## Usage

Place your mods in the mods_profile folder and your overrides in the mod_overrides_profile folder, profile being the name of the profile you want to use. You may want to launch the game once to generate the folders or create them yourself. Then launch the launcher and select the profile you want to use.

:warning: Do NOT change the original mods folder, as it will overwrite the mods in the profile folder.
You may want to do a backup of your mods folder before using this launcher as it is still in development and may not work as intended.

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.
You will need to install Qt Creator and all Qt dependencies to compile the project and make changes to the UI.
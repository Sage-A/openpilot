# Software Overview - Custom GUI

Installation of the custom GUI on the Comma3x device is documented in detail in the User Manual. This setup is to run Openpilot on a laptop instead of a comma or for further development.

*System Requirements: Ubuntu 24.04, near-native macOS, WSL Ubuntu environment on Windows. Other systems are not recommended*

## Setup (Ubuntu, macOS)
1. Install and set up Git LFS
2. Clone openpilot `git clone --recurse-submodules https://github.com/<username>/openpilot.git`
3. Run setup script `cd openpilot` `tools/op.sh setup`
5. Git LFS pull `git lfs pull`
6. Activate python shell `source .venv/bin/activate`
7. Build openpilot `scons -u -j$(nproc)`
8. Run openpilot `./tools/sim/launch_openpilot.sh`

**For Windows: Use WSL and install Ubuntu-24.04 then follow setup instructions**

### Development Notes:
After making changes to the software, run `git pull` followed by steps 6 & 7 to rebuild and run openpilot

After exiting the UI window, a process will still be running in the console. `Ctrl-C` will interrupt it and exit.

To install a new python library for use, modify openpilot/pyproject.toml dependencies to force a library to be installed during the build process. To force project dependencies to update, modify openpilot/tools/install_python_dependencies.sh to include `uv lock` prior to `uv sync --frozen --all-extras` . 

Openpilot only needs to be built once and the subsequently produced uv.lock file saved to GitHub to force the inclusion of the new dependency and `uv lock` can be removed for future builds.

### File Tree:
- selfdrive/ui/
  - SConscript
  - qt/
    - widgets/
	    - controls.cc
	    - controls.h
    - offroad/
	    - settings.h
	    - settings.cc
	  	- customGUI.h
	  	- customGUI.cc
  		- custom_widgets/
  			- acceleration.cc
  			- acceleration.h
  			- status.cc
  			- status.h
	  		- steering.h
	  		- steering.cc

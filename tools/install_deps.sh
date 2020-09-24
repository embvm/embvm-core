#!/bin/bash
#
# This script takes two optional arguments.
# -u will run an update process instead of install process
# -e will modify enviroment files during the install process
#
# You can override the installation path for toolchains by defining TOOLCHAIN_INSTALL_DIR
# You can override the installation path for tools by defining TOOL_INSTALL_DIR
# If the toolchain or tools directories do not require sudo permissions, disable the use
# of sudo by defining TOOLCHAIN_DISABLE_SUDO=1 or TOOL_DISABLE_SUDO=1
#

STARTING_DIR=$PWD
TOOLCHAIN_INSTALL_DIR=${TOOLCHAIN_INSTALL_DIR:-/usr/local/toolchains}
TOOL_INSTALL_DIR=${TOOL_INSTALL_DIR:-/usr/local/tools}
TOOLCHAIN_DISABLE_SUDO=${TOOLCHAIN_DISABLE_SUDO:-0}
TOOL_DISABLE_SUDO=${TOOL_DISABLE_SUDO:-0}

# Commands
UPDATE=0
UPDATE_ENV=0
PIP_UPDATE=
BREW_COMMAND="install"
APT_COMMAND="install"
TOOLS_SUDO=sudo
if [ $TOOL_DISABLE_SUDO == 1 ]; then
	TOOLS_SUDO=
fi

# Packages to Install
BREW_PACKAGES=("python3" "ninja" "wget" "gcc@7" "gcc@8" "gcc@9" "llvm" "adr-tools" "cmocka" "pkg-config")
BREW_PACKAGES+=("vale" "doxygen" "cppcheck" "clang-format" "gcovr" "lcov" "sloccount")
# TODO: test app - do I need to add anything for versions?
APT_PACKAGES=("python3" "python3-pip" "ninja-build" "wget" "build-essential" "clang" "lld" "llvm")
APT_PACKAGES+=("clang-tools" "libcmocka0" "libcmocka-dev" "pkg-config" "sloccount" "curl")
APT_PACKAGES+=("doxygen" "cppcheck" "gcovr" "lcov" "clang-format" "clang-tidy" "clang-tools")
APT_PACKAGES+=("gcc-7" "g++-7" "gcc-8" "g++-8" "gcc-9" "g++-9")
PIP3_PACKAGES=("meson" "lizard" "thefuck")

while getopts "euh" opt; do
  case $opt in
  	e) UPDATE_ENV=1
	;;
	u)	UPDATE=1
		PIP_UPDATE="--upgrade"
		BREW_COMMAND="upgrade"
		APT_COMMAND="upgrade"
	;;
	h) # Help
		echo "Usage: install_deps.sh [optional ags]"
		echo "Optional args:"
		echo "	-u: Run an update instead of install"
		echo "  -e: Include environment setup during install process (.bashrc + .bash_profile)"
		exit 0
	;;
    \?) echo "Invalid option -$OPTARG" >&2
    ;;
  esac
done

# Shift off the getopts args, leaving us with positional args
shift $((OPTIND -1))

if [ "$(uname)" == "Darwin" ]; then
	if [ $UPDATE == 1 ]; then
		# update homebrew
		brew update
	else
		#install brew if unavailable
		if [ -z "$(command -v brew)" ]; then
			/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
		fi
		# Needed for GCC compiler versions
		brew tap homebrew/cask-versions
	fi

	brew ${BREW_COMMAND} ${BREW_PACKAGES[@]}
	pip3 install ${PIP3_PACKAGES[@]} ${PIP_UPDATE}
else
	# WSL/Linux Case
	${TOOLS_SUDO} apt-get update
	${TOOLS_SUDO} apt ${APT_COMMAND} -y ${APT_PACKAGES[@]}
	# Install pip3 dependencies globally, not just for the current user
	${TOOLS_SUDO} -H pip3 install ${PIP3_PACKAGES[@]} ${PIP_UPDATE}

	# Install Vale
	cd /tmp
	wget https://install.goreleaser.com/github.com/ValeLint/vale.sh
	${TOOLS_SUDO} sh vale.sh -b /usr/local/bin
	rm vale.sh

	# Install adr-tools
	if [ $UPDATE == 1 ]; then
		cd ${TOOL_INSTALL_DIR}/adr-tools
		${TOOLS_SUDO} git pull
	else
		${TOOLS_SUDO} mkdir -p ${TOOL_INSTALL_DIR}
		cd ${TOOL_INSTALL_DIR}
		${TOOLS_SUDO} git clone https://github.com/npryce/adr-tools.git --recursive
	fi
fi

###########################
# Common Dependency Steps #
###########################

# Install gcc-arm-none-eabi
if [ $UPDATE == 0 ]; then
	# Assume that the two scripts are contained in the same directory
	cd $STARTING_DIR
	source $(dirname $0)/install_arm_gcc.sh
fi

# Install Pottery
if [ $UPDATE == 1 ]; then
	cd ${TOOL_INSTALL_DIR}/pottery
	${TOOLS_SUDO} git pull
else
	${TOOLS_SUDO} mkdir -p ${TOOL_INSTALL_DIR}
	cd ${TOOL_INSTALL_DIR}
	${TOOLS_SUDO} git clone https://github.com/npryce/pottery.git --recursive
fi

#############################
# Install Environment Files #
#############################

if [ $UPDATE == 0 ]; then
	if [ $UPDATE_ENV == 1 ]; then
		# Assume that the two scripts are contained in the same directory
		cd $STARTING_DIR
		source $(dirname $0)/setup_env.sh
	else
		echo "You may need to manually modify your PATH to reference the programs in $TOOLCHAIN_INSTALL_DIR and $TOOL_INSTALL_DIR"
	fi
fi

# End in the starting directory
cd $STARTING_DIR

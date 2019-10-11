# C++ Basics: Conway's Game of Life

## Requirements
- SFML: sfml-graphics

## Get the Code and Start Developing

    mkdir cpp-basics
    cd cpp-basics
    git clone https://github.com/lyrahgames/cpp-basics-game-of-life.git game-of-life
    cd game-of-life

**Fast Building:**
In-source build without persistent configuration:
    
    # Build the executable with ad-hoc configuration and compiler flags.
    b config.cxx=clang++ "config.cxx.coptions=-O3 -march=native"
    # Run the application.
    game-of-life/game-of-life
    # Clean build output.
    b clean

**Persistent Building:**
Out-of-source build with persistent configuration:

    # Create the persistent configuration out-of-source and set appropiate compiler flags.
    bdep init -C @gcc cc config.cxx=g++ "config.cxx.coptions=-O3 -march=native"
    # Build the executable.
    b
    # Run the executable.
    game-of-life/game-of-life
    # Clean build output.
    b clean

From here on, open your editor inside the directory and look at the code and start with further developments.


## Get the Package and Install the Application

    # Go somewhere to install the package.
    mkdir ~/builds
    cd ~/builds
    # Create persistent configuration for all packages in the new directory
    # 'cpp-basics-packages' with appropriate compiler flags and installation options.
    bpkg create -d cpp-basics-packages cc \
        config.cxx=clang++ \
        "config.cxx.coptions=-O3 -march=native" \
        config.install.root=/usr/local \
        config.install.sudo=sudo
    cd cpp-basics-packages
    # Download and build this packages inside the configuration folder.
    bpkg build game-of-life@https://github.com/lyrahgames/cpp-basics-game-of-life.git#master
    # Install the built executable with its documentation.
    bpkg install game-of-life
    # Run the application in any directory.
    game-of-life
    # Uninstall the executable while in 'cpp-basics-packages'.
    bpkg uninstall game-of-life



## Usage

    game-of-life/game-of-life [<rows> <cols> [<option>]]

- Left Mouse Move: Move the origin.
- Left Mouse Click: Toggle cell.
- Mouse Wheel: Zoom in and out.
- c: Clear all cells.
- r: Initialize cells randomly.
- v: Set adaptive view.
- Space: Advance the Game of Life state.
- Escape: Close the window.

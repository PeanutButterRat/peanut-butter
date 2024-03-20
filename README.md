<a name="readme-top"></a>


[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


<!-- PROJECT LOGO -->
<br />
  <div align="center">

```

 ____  ____    __    _  _  __  __  ____    ____  __  __  ____  ____  ____  ____ 
(  _ \( ___)  /__\  ( \( )(  )(  )(_  _)  (  _ \(  )(  )(_  _)(_  _)( ___)(  _ \
 )___/ )__)  /(__)\  )  (  )(__)(   )(     ) _ < )(__)(   )(    )(   )__)  )   /
(__)  (____)(__)(__)(_)\_)(______) (__)   (____/(______) (__)  (__) (____)(_)\_)


```
  <i><p align="center"> ASCII art was created with the help of <a href="https://patorjk.com/software/taag/">patorjk.com</a></p></i>
  
  <br>
  <h3 align="center">Peanut Butter</h3>
  
  <p align="center">
    A C++ implementation of the <i>prestigious</i> Peanut Butter Programming Language. 
    <br />
    <br />
    <a href="https://github.com/PeanutButterRat/peanut-butter/issues">Report Bug</a>
    ·
    <a href="https://github.com/PeanutButterRat/peanut-butter/issues">Request Feature</a>
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

Peanut Butter is my attempt at a dynamically-typed, imperative programming language. It's about as simple as it can get but is also a neat little tool to play around with. It compiles source files into its own bytecode which then runs on a interpreter. Peanut Butter has a syntax like Python, but a compilation process like Java.

It's not going to compete with any professional programming langauge or make its way into any development workflows, but it has some interesting ideas that will keep you busy for a little while.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To get Peanut Butter up and running, follow these simple steps shown below.

### Prerequisites

For starters, you will need to download some dependencies.
1. [CMake](https://cmake.org/), Version 3.27+. You can probably get away with a different version, just be sure to change the first line of `CMakeLists.txt`.
2. [Visual Studio](https://visualstudio.microsoft.com/downloads/)

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/PeanutButterRat/peanut-butter.git
   ```

2. Run the build script
   ```sh
   cd peanut-butter  # You should now be in the repo directory.
   ./build.sh  # Build the project.
   ```
3. Once compiled, you can run the automated tests
   ```sh
   ./tests.sh
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

To use Peanut Butter, create a file with a `.pb` extension with the code you want compiled in it.

```sh
touch hello_world.pb
echo 'print "Hello, World!".' > hello_world.pb  # A simple "Hello, World!" application.
```

Compile the program with `cpbpl`.
```sh
./bin/cpbpl hello_world.pb  # Assuming you are in base directory for the repo.
```

Execute the compiled program with `pbpl`.
```sh
./bin/pbpl hello_world.nut  # Notice the file extension change.
```

<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with and present your idea there. I don't forsee anyone feeling inspired enough to contribute, but any contributions are greatly appreciated if you do decide to do so!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Eric Brown - [GitHub](https://github.com/PeanutButterRat) - ebrown5676@gmail.com

Project Link: [https://github.com/PeanutButterRat/peanut-butter](https://github.com/PeanutButterRat/peanut-butter)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

The only major resource used in this project was the knowledge found in _Crafting Interpreters by Robert Nystrom_. The book is very well written, and got me hooked on language design.
Peanut Butter is largely its own creation and doesn't follow the source code given in the book but rather it relies on the theory presented by the author for implementing a language. 

You can find more it on the author's website at [https://craftinginterpreters.com/](https://craftinginterpreters.com/).


<p align="right">(<a href="#readme-top">back to top</a>)</p>


[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/PeanutButterRat/peanut-butter/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/eric-brown-b0a258202/

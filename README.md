# SysMon


## Verification


- [x] `README.md` with build instructions
- [x] Builds on Linux / Linux VM
- [x] Used one of the allowed programming languages (C, C++, Rust, Zig, Golang)
- [x] Monitor CPU
	- [x] Current total usage
	- [x] Current usage per application
	- [x] Current free
- [x] Monitor Memory
	- [x] Current total usage
	- [x] Current total free
	- [x] Current usage per application
- [x] Monitor Disk
	- [x] Available devices
	- [x] Available partitions
	- [x] Read rate per application
	- [x] Write rate per application
- [x] Real-time stats collection
- [ ] UI (check any or both)
	- [x] Terminal-based UI
	- [ ] Prometheus + Grafana




## Build instructions

### Prerequisite
List of build/run-time dependencies:
  * `clang`
  * `make`
  * `ncurses`

Install these packages for C development from your package manager.

**Debian/Ubuntu**
~~~ shell
sudo apt install libncursesw5-dev make build-essential clang
~~~

**Fedora/RHEL**
~~~ shell
sudo dnf install clang ncurses-devel make
~~~

**Archlinux/Manjaro**
~~~ shell
sudo pacman -S ncurses clang ncurses make
~~~

**macOS**
~~~ shell
brew install clang ncurses make 
~~~

### Compile & Run:
To compile and run Sysmon
~~~ shell
make && ./sysmon
~~~
> Pleas consider using wide enough Terminal screen for good output.


## License

GNU General Public License, version 2 (GPL-2.0) or, at your option, any later version.

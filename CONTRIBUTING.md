## Contributing to TimerInterrupt

### Reporting Bugs

Please report bugs in TimerInterrupt if you find them.

However, before reporting a bug please check through the following:

* [Existing Open Issues](https://github.com/khoih-prog/TimerInterrupt/issues) - someone might have already encountered this.

If you don't find anything, please [open a new issue](https://github.com/khoih-prog/TimerInterrupt/issues/new).

### How to submit a bug report

Please ensure to specify the following:

* Arduino IDE version (e.g. 1.8.19) or Platform.io version
* `Arduino AVR` or `Adafruit AVR` Core Version (e.g. Arduino AVR core v1.8.6 or Adafruit AVR Core v1.4.15)
* Contextual information (e.g. what you were trying to achieve)
* Simplest possible steps to reproduce
* Anything that might be relevant in your opinion, such as:
  * Operating system (Windows, Ubuntu, etc.) and the output of `uname -a`
  * Network configuration


### Example

```
Arduino IDE version: 1.8.19
Arduino AVR Core Version 1.8.6
OS: Ubuntu 20.04 LTS
Linux xy-Inspiron-3593 5.15.0-53-generic #59~20.04.1-Ubuntu SMP Thu Oct 20 15:10:22 UTC 2022 x86_64 x86_64 x86_64 GNU/Linux

Context:
I encountered a crash while using this library
Steps to reproduce:
1. ...
2. ...
3. ...
4. ...
```

### Additional context

Add any other context about the problem here.

---

### Sending Feature Requests

Feel free to post feature requests. It's helpful if you can explain exactly why the feature would be useful.

There are usually some outstanding feature requests in the [existing issues list](https://github.com/khoih-prog/TimerInterrupt/issues?q=is%3Aopen+is%3Aissue+label%3Aenhancement), feel free to add comments to them.

---

### Sending Pull Requests

Pull Requests with changes and fixes are also welcome!

Please use the `astyle` to reformat the updated library code as follows (demo for Ubuntu Linux)

1. Change directory to the library GitHub

```
xy@xy-Inspiron-3593:~$ cd Arduino/xy/TimerInterrupt_GitHub/
xy@xy-Inspiron-3593:~/Arduino/xy/TimerInterrupt_GitHub$
```

2. Issue astyle command

```
xy@xy-Inspiron-3593:~/Arduino/xy/TimerInterrupt_GitHub$ bash utils/restyle.sh
```



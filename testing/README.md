## Testing

Currently working on (Target System):
- Linux 5.4.0-97-generic x86_64 GNU/Linux

**Contribute tested devices:**  
If your testing script ran successfully, I would appreciate if you could send me your `uname -srmo` of both Host and target system via a new Issue or email me. Thank you :)

To test, if all keys are send correctly, you can copy `testing/testing-check` to your target-machine and execute `./pineDucky testing/testing-script out-testing.sh` on your Pinephone, while your Pinephone is connected and gadget-mode is enabled. If you didn't enable Gadget-Mode already, follow the instructions on the main **README.md** and then come back here.

Then have a look on your target-machine and if there is no output of the `diff` command that is run, everything worked fine :)

# minishell

jorexpos & gade-oli mini :(Sh)

all relevant info regarding its development is stored on the Wiki of this repo

## check leaks

to suppress leaks generated by the readline and add_history on the valgrind report, compile and use:

```bash
valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
```
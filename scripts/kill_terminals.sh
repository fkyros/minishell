#!/bin/bash

# COLOURS
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
BOLD='\033[1m'

# Send signal to end minishell
echo "${RED}${BOLD}Killing minishell >:)${NC}"
pkill -f "./minishell" --signal SIGTERM

# End tmux
echo "${RED}${BOLD}Killing tmux >:)${NC}"
tmux kill-server 2>/dev/null

# Done! :D
echo "${GREEN}${BOLD}DONE >:)${NC}"

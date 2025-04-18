#!/bin/bash

# 🎨 Paleta de colores
PURPLE='\033[0;35m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m'
BOLD='\033[1m'

SESSION="minishell"
MINISHELL_PATH="./minishell"
# Create the testing directory within the directory that contains the minishell program
TEST_DIR="$(pwd)/MinishellTesting"

# Check whether the executable exists within the directory or not
if [[ ! -x "$MINISHELL_PATH" ]]; then
  echo -e "${RED}${BOLD}Error: '$MINISHELL_PATH' couldn't be found or isn't an executable!!${NC}"
  echo -e "${PURPLE}${BOLD}Make sure to execute this script in the directory that contains the minishell program!!${NC}"
  exit 1
fi

# We create the testing directory in case it doesn't exist
if [[ ! -d "$TEST_DIR" ]]; then
  mkdir -p "$TEST_DIR"
fi

# We cleanup the testing directory in case it contains something
rm -rf "$TEST_DIR"/*

# Create a new tmux session as a background process
tmux new-session -d -s $SESSION

# Execute minishell on the first pane (left one)
tmux send-keys -t $SESSION "$MINISHELL_PATH" C-m

# Split terminals vertically
tmux split-window -h -t $SESSION

# Switch to testing directory and execute bash (in the second terminal, right one)
tmux send-keys -t ${SESSION}:0.1 "cd $TEST_DIR && bash" C-m

# Synchronize both terminals
tmux set-window-option -t $SESSION synchronize-panes on

# Select the first pane (left one, minishell) as the active one 
# Can change doing Ctrl+B and change active pane moving right and left keys
tmux select-pane -t 0

# Cleanup function
cleanup() 
{
  rm -rf "$TEST_DIR"
  echo -e "${PURPLE}${BOLD}Testing directory ${CYAN}'$TEST_DIR' ${RED}obliterated >:)${NC}"
}

# Call cleanup function on exit
trap cleanup EXIT

# Attach session to current terminal so we can actually see it
tmux attach-session -t $SESSION


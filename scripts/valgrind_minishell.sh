 #!/bin/bash

# COLOURS
PURPLE='\033[0;35m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m'
BOLD='\033[1m'

clean_valgrind_log() {
    local log_file="$LOG_DIR/full_valgrind_output.txt"
    local cleaned_log="$LOG_DIR/full_valgrind_output_clean.txt"

    # Get all PIDs of external commands (like /usr/bin/ls)
    grep -oP '^==\K\d+(?=== Command: (?!\./minishell))' "$log_file" > "$LOG_DIR/external_pids.txt"

    # Remove lines belonging to external PIDs
    grep -vFf "$LOG_DIR/external_pids.txt" "$log_file" > "$cleaned_log"

    # Replace original log with cleaned version
    mv "$cleaned_log" "$log_file"
    rm -f "$LOG_DIR/external_pids.txt"
}

# Logs directory
LOG_DIR="valgrind_logs"

# Clean previous log directory if it existed
if [ -d "$LOG_DIR" ]; then
    rm -rf "$LOG_DIR"
fi
mkdir -p "$LOG_DIR"

# Check if the minishell executable exists
if [ ! -x "./minishell" ]; then
    echo -e "${RED}Error:${NC} couldn't find an executable named minishell :("
    exit 1
fi

# Execute valgrind
valgrind --suppressions=readline.supp \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --trace-children=yes \
         --child-silent-after-fork=yes \
         --log-file="$LOG_DIR/full_valgrind_output.txt" \
         ./minishell "$@"

# Clean the log by removing external command entries
clean_valgrind_log

# Get ./minishell's PID
MAIN_PID=$(grep -m1 "HEAP SUMMARY" "$LOG_DIR/full_valgrind_output.txt" | grep -oP "==\K\d+(?==)")

if [[ -z "$MAIN_PID" ]]; then
    echo -e "${RED}Error:${NC} couldn't identify minishell's PID"
    exit 1
fi

# Get ./minishell (parent process) logs
grep "^==${MAIN_PID}==" "$LOG_DIR/full_valgrind_output.txt" > "$LOG_DIR/valgrind_output.txt"

# Main valgrind summary (./minishell)
echo -e "\n${PURPLE}${BOLD}=========== MAIN LOG (PID $MAIN_PID ${RED}[./minishell]${PURPLE}${BOLD}) ===========${NC}"
grep -E "LEAK SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable" "$LOG_DIR/valgrind_output.txt" | while read -r line; do
    case "$line" in
        *"LEAK SUMMARY"*)  echo -e "${GREEN}$line${NC}" ;;
        *"definitely lost:"*) echo -e "${RED}$line${NC}" ;;
        *"indirectly lost:"*|*"possibly lost:"*) echo -e "${YELLOW}$line${NC}" ;;
        *"still reachable:"*) echo -e "${CYAN}$line${NC}" ;;
    esac
done

# Process children traces
echo -e "\n${PURPLE}${BOLD}==================== CHILDREN LOG =======================${NC}"
> "$LOG_DIR/valgrind_children_log.txt"

# Get children's logs
grep -oP '^==\K\d+(?=== Command: )' "$LOG_DIR/full_valgrind_output.txt" | while read -r child_pid; do
    if [[ "$child_pid" != "$MAIN_PID" ]]; then
        # Get the command this child executed (e.g., ./minishell or /usr/bin/ls)
        CHILD_CMD=$(grep -m1 "^==${child_pid}== Command: " "$LOG_DIR/full_valgrind_output.txt" | sed 's/.*Command: //')
        
        # Only show leaks if the child is running minishell (i.e., a builtin)
        if [[ "$CHILD_CMD" == "./minishell" ]]; then
            # Write in children's log
            grep "^==${child_pid}==" "$LOG_DIR/full_valgrind_output.txt" >> "$LOG_DIR/valgrind_children_log.txt"
            
            # Show children summary
            echo -e "\n${YELLOW}CHILD PID $child_pid:${NC}"
            grep -E "LEAK SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable|blocks are still reachable" <(grep "^==${child_pid}==" "$LOG_DIR/full_valgrind_output.txt") | while read -r line; do
                case "$line" in
                    *"LEAK SUMMARY"*) echo -e "${GREEN}$line${NC}" ;;
                    *"definitely lost:"*) echo -e "${RED}$line${NC}" ;;
                    *"indirectly lost:"*|*"possibly lost:"*) echo -e "${YELLOW}$line${NC}" ;;
                    *"still reachable:"*|*"blocks are still reachable"*) echo -e "${CYAN}$line${NC}" ;;
                esac
            done
        else
            echo -e "\n${YELLOW}Skipping external command: $CHILD_CMD${NC}"
        fi
    fi
done
# Done!! :D
echo -e "\n${PURPLE}✔ All logs were saved to a directory named: ${BOLD}$LOG_DIR/${RED} :) ${NC}\n"

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 15:20:40 by rrouille          #+#    #+#              #
#    Updated: 2023/05/19 12:01:58 by rrouille         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the executable
NAME			= philo
NAMEBNS			= philo_bonus

# Arguments passed to the executable
ARGS			= ${filter-out $@,${MAKECMDGOALS}}
PRINT_SCREEN	= YES

# Directories
SRCSDIR			= ./
OBJSDIR			= objs
HDRDIR			= ./
SRCSBNSDIR		= ./philo_bonus
OBJSBNSDIR		= objs/philo_bonus

# Colors for the terminal
GRAY			= \033[0;90m
RED				= \033[0;91m
GREEN			= \033[0;92m
YELLOW			= \033[0;93m
BLUE			= \033[0;94m
MAGENTA			= \033[0;95m
CYAN			= \033[0;96m
WHITE			= \033[0;97m
ENDCOLOR		= \033[0m

# Background colors
BG_G			= \033[42m
BG_R			= \033[41m
BG_Y			= \033[43m
BG_B			= \033[44m
BG_M			= \033[45m
BG_C			= \033[46m
BG_W			= \033[47m

# Text formatting
BOLD			= \033[1m
UNDERLINE		= \033[4m
ITALIC			= \033[3m
INVERTED		= \033[7m

# Clear screen
CLEAR			= \033c

# Sources
SRCS			= ${shell find ${SRCSDIR} -maxdepth 1 -type f -name '*.c'}
OBJS			= ${patsubst ${SRCSDIR}%,${OBJSDIR}%,${SRCS:%.c=%.o}}
SRCSBNS			= ${shell find ${SRCSBNSDIR} -type f -name '*.c'}
OBJSBNS			= ${patsubst ${SRCSBNSDIR}%,${OBJSBNSDIR}%,${SRCSBNS:%.c=%.o}}
CFLAGS			= -Werror -Wall -Wextra -g
CC				= gcc
RM				= rm -rf
MAKE			= make
MKDIR			= mkdir -p

# Operating System
OS				:= ${shell uname}

# Progress bar messages
START			= echo "${YELLOW}\n🚀 Start of program compilation 🚀${ENDCOLOR}"
END_COMP		= echo "${GREEN}\n\n✅ Compilation completed successfully! ✅${ENDCOLOR}"
S_OBJS			= echo "${RED}🧹 Cleaning objects... 🧹${ENDCOLOR}"
S_NAME			= echo "${RED}🧹 Cleaning program... 🧹${ENDCOLOR}"
CHARG_LINE		= echo "${BG_G} ${ENDCOLOR}\c" && sleep 0.05
BS_N			= echo "\n"

# First rule
all:	 draw_begining ${NAME} draw_ready

# Build rule for object files
${OBJSDIR}/%.o : ${SRCSDIR}/%.c
			@${MKDIR} ${OBJSDIR}
			@${CC} ${CFLAGS} -I ${HDRDIR} -c $< -o $@

# Build rule for bonus object files
${OBJSBNSDIR}/%.o : ${SRCSBNSDIR}/%.c
			@${MKDIR} ${OBJSBNSDIR}
			@${CC} ${CFLAGS} -I ${HDRDIR} -c $< -o $@

# Linking rule
${NAME}: ${OBJS}
			@${CHARG_LINE}
			@for i in ${OBJS} ${C_NOT_LAST}; do \
				${CHARG_LINE}; \
			done;
			@${CHARG_LINE} ${C_LAST};
			@${CC} ${CFLAGS} ${OBJS} -o ${NAME}
			@${END_COMP}
			@sleep 0.5

# Run the program
run:	clear ${NAME}
#	@${call shuffle_animation}
			@echo "${CLEAR}${GREEN}🔧 Operations completed: 🔧${ENDCOLOR}"
			@./${NAME} ${ARGS}
r:		clear ${NAME}
			@echo "${CLEAR}${GREEN}🔧 Operations completed: 🔧${ENDCOLOR}"
			@./${NAME} ${ARGS}

# Tests
test:
			@echo "${CLEAR}${GREEN}✨ Result of checker: ✨${ENDCOLOR}"
			@cd tester && bash loop.sh ${ARGS}
t:
			@echo "${CLEAR}${GREEN}✨ Result of checker: ✨${ENDCOLOR}"
			@cd tester && bash loop.sh ${ARGS}

# Check if the program is correct
check:	clear ${NAME}
			@echo "${CLEAR}${GREEN}✨ Result of checker: ✨${ENDCOLOR}"
			@./${NAME} ${ARGS} | ./checker_Mac ${ARGS}
c:		clear ${NAME}
			@echo "${CLEAR}${GREEN}✨ Result of checker: ✨${ENDCOLOR}"
			@./${NAME} ${ARGS} | ./checker_Mac ${ARGS}

# Bonus
bonus:	draw_bonus ${OBJSBNS}
			@if [ -z "${BONUS_OBJS}" ]; then \
				echo "${RED}⚠️ Sorry, bonuses are not available yet... ⚠️\n${RESET}"; \
			else \
				${CHARG_LINE}; \
				for i in ${OBJSBNS} ${C_NOT_LAST}; do \
					${CHARG_LINE}; \
				done; \
				${CHARG_LINE} ${C_LAST}; \
				${CC} ${CFLAGS} ${OBJSBNS} -o ${NAMEBNS}; \
				${END_COMP}; \
			fi
			@sleep 0.3
b:		bonus

###############################################################################
#                   ↓↓↓↓↓           CLEANING           ↓↓↓↓↓                  #
###############################################################################

# Clean object files and executable
clean:
			@echo "${CLEAR}"
			@${S_OBJS}
			@${RM} objs/
			@sleep 0.3
			@echo "${CLEAR}"
			@echo "${GREEN}✅ Simple clean completed! ✨\n"

# Clean everything
fclean: clean
			@${S_NAME}
			@${RM} ${NAME}
			@sleep 0.3
			@echo "${CLEAR}"
			@echo "${GREEN}✅ Deep clean completed! ✨"

###############################################################################
#                  ↓↓↓↓↓           UTILITIES           ↓↓↓↓↓                  #
###############################################################################

# Drawings
draw_begining:
			@echo "${CLEAR}${CYAN}\c"
			@cat ascii_art/title
			@sleep 0.3
			@echo "${CLEAR}${GRAY}\c"
			@cat ascii_art/by
			@sleep 0.3
			@echo "${CLEAR}${GREEN}\c"
			@cat ascii_art/owner
			@sleep 0.3
			@echo "${CLEAR}"

draw_bonus:
			@echo "${CLEAR}"
			@echo "${BLUE}"
			@echo "                         ###         ###      ###    \`#    \`##+    .#\'         :##:       \'##"
			@sleep 0.01
			@echo "               :;;;;;;;;;\`@@#  ;;;;;;\`@@# ;;; @@\`;;;\`#\`;;:\`#@#\`;;:.+\`.;;;;;;;;,;@@:   ,;;.+@@"
			@sleep 0.01
			@echo "               ;;;\` ;;;;; @@\`;;;;;;;;;\`@@ ;;; @@ ;;;\`#\`;;;\`@@#\`;;:.+\`;;: .;;;;,;@@:   ,;;.+@@"
			@sleep 0.01
			@echo "              ;;;\`#@\`;;;\`#\` ;;;;;;;;;; @ ;;;; @\`;;;\`#\`;;;\`@@#\`;;:.+.;;:,@\'.;;,:@@:   ,;;.\'@@."
			@sleep 0.01
			@echo "              ;;;\`#@\`;;; @ ;;;   ;;;;; @ ;;;;; ;;;; # ;;;\`@@#\`;;:.+\`;;:,@\'    :@@:   ,;;.\'@@."
			@sleep 0.01
			@echo "             :;;\`#@\`;;;\`#\`;;; @@@ ;;; @ ;;;;;;;;;;\`#\`;;;\`@@#\`;;;.+\`;;:,@@\'          ,;;.\'@@."
			@sleep 0.01
			@echo "             :;;;\` ;;;; @ ;;; @@@ ;;; @ ;;;;;;;;;; @ ;;;\`@@#\`;;;.+\`;;;:     :@@:    ,;;.\'@@."
			@sleep 0.01
			@echo "            :;;;;;;;;; # ;;; @@@ ;;; @ ;;;;;;;;;;\`@ ;;;\`@@#\`;;;\`+\`;;;;;;;;;,:@@;   ,;;.\'@@."
			@sleep 0.01
			@echo "            ;;;\` ;;;;; # ;;; @@@ ;;; @ ;;;;;;;;;; @ ;;;\`@@#\`;;;\`+     .;;;;,:@@:   ,;;.\'@@."
			@sleep 0.01
			@echo "           :;;\`#@\`;;; #\`;;; @@@ ;;; @ ;;;;;;;;;; @ ;;;\`@@#\`;;;\`@@+     .;;,:@@;   ,;;.\'@@,"
			@sleep 0.01
			@echo "           :;;\`#@\`;;; #\`;;; @@@ ;;; @ ;;;;;;;;;; @ ;;;\`@@#\`;;;\`@@+     .;;,:@@;   ,;;.\'@@,"
			@sleep 0.01
			@echo "          :;;\`#@\`;;;\`#\`;;; @@@ ;;; @ ;;;; ;;;;; @ ;;;\`@@#\`;;;\`@@+     .;;,:@@;   ,;;.\'@@,"
			@sleep 0.01
			@echo "          :;;\`#@\`;;; #\`;;;;   ;;;; @ ;;; @ ;;;; @ ;;;\`@@#\`;;;\`#    .@+.;;,:@@;       \'@@,"
			@sleep 0.01
			@echo "         :;;;;\` ;;;\`#\`;;;;;;;;;;  @ ;;; @@ ;;; @ ;;;;;  \`;;;\`#\`;;;;: .;;:,@@;"
			@sleep 0.01
			@echo "         :;;;;;;;;;\`#@\`;;;;;;;; @@@ ;;; @@ ;;; @ ;;;;;;;;;;;\`#\`;;;;;;;;: ,@@;"
			@sleep 0.01
			@echo "        :;;;;;;;;;\`#@\`  ;;;;;; @@@ ;;; @@@ ;; @@@ ;;;;;;;; \`@#\`;;;;;;;:,@@\'        ;@@,"
			@sleep 0.01
			@echo "                  #@\`         @@@      @@@    @@@        \`@@#          ,@@\'        ;@@,"
			@sleep 0.3
			@echo "${ENDCOLOR}"

draw_ready:
			@echo "${CLEAR}${GREEN}${BOLD}\c"
			@cat ascii_art/prog_ready
			@echo "${ENDCOLOR}"
			@make help PRINT_SCREEN=NO

# Build rule for help function
help:
			@if [ "${PRINT_SCREEN}" = "YES" ]; then \
				echo "${CLEAR}\c"; \
			fi
			@echo "${GRAY}🏃 Run ${ITALIC}\`./${NAME} <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY} to start the program. 🚀\n"
			@echo "${BOLD}${UNDERLINE}💡 TIPS: 💡${ENDCOLOR}${GRAY}"
			@echo "\t- You can also use ${ITALIC}\`make run <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make r <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY} to try it out."
			@echo "\t- Use a tester with ${ITALIC}\`make test <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make t <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY}."
			@echo "\t- Check for memory leaks with ${ITALIC}\`make leaks <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make l <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> {<number_of_times_each_philosopher_must_eat>}\`${ENDCOLOR}${GRAY}."
			@echo "\t- Check the 42 norm with ${ITALIC}\`make norm\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make n\`${ENDCOLOR}${GRAY}."
			@echo ""
			@echo "\t- Bonus? Try it with ${ITALIC}\`make bonus\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make b\`${ENDCOLOR}${GRAY} 🌟."
			@echo ""
			@echo "${YELLOW}🌟 Use ${ITALIC}\`make help\`${ENDCOLOR}${YELLOW} or ${ITALIC}\`make h\`${ENDCOLOR}${YELLOW} to display these helpful tips. 🚀${ENDCOLOR}"
h:		help

# Norminette
norm:
			@norminette srcs && norminette includes
n:		norm

# Leaks
leaks:	clear ${NAME}
			@echo "${CLEAR}\c"
			@leaks -atExit -- ./${NAME} ${ARGS}
l:		clear ${NAME}
			@echo "${CLEAR}\c"
			@leaks -atExit -- ./${NAME} ${ARGS}

# Run the program with lldb
lldb:	clear ${NAME}
			@echo "${CLEAR}"
			@lldb ./${NAME} ${ARGS}
			@echo "${CLEAR}"

# Push the files to Git
git:	fclean
			@echo "${CLEAR}"
			@git add .
			@echo "${CYAN}✅ Added files to git! 📁"
			@git commit -m "💻 Auto-commit"
			@echo "${BLUE}✅ Changes committed! ✔️"
			@git push
			@echo "${GREEN}✅ All changes are now on GitHub! 🚀${ENDCOLOR}"

# Dummy target to prevent arguments with dashes from being interpreted as targets
%:
			@:

# Clear the screen
clear:
			@echo "${CLEAR}\c"

# Rebuild the program
re: fclean all

.PHONY: all clean fclean re run test
#ifndef DEFINES_HPP
#define DEFINES_HPP

// Settings:
#define BOT "procs"
#define LOCALHOST "127.0.0.1"

// Colors:
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// JOIN/PART msgs:
#define WELCOME(channel) " :Welcome to " + channel + " hope u like it here type /help in u need help."
#define FAREWELL(channel) " :hope i served u well come back soon " BOT


#define MANUAL  {                                                                                                       \
                    " :/help                 : Displays this help message.\r\n",                                        \
                    " :/joke                 : Tells a random joke.\r\n",                                               \
                    " :/toss                 : Tosses a coin: head or tails.\r\n",                                      \
                    " :/weather {city}       : Gets weather info from the API for the specified city.\r\n",             \
                    " :/admin {option} {arg} : Admin command with options:\r\n",                                        \
                    " :      - add  {user}         : Add a user to the master list.\r\n",                               \
                    " :      - del  {user}         : Remove a user from the master list.\r\n",                          \
                    " :      - join {channel}      : Join the specified channel.\r\n",                                  \
                    " :      - part {channel}      : Leave the specified channel.\r\n",                                 \
                    " :      - set  {api_key}      : Set a The api key for" HOST ".\r\n",                               \
                    " :      - display {masters/channels}    : List masters or channels the bot is currently in.\r\n",  \
                }
// " :/poll {option} {arg}  : Poll command with options:",                      
// " :     - list                 : List all available polls.",                 
// " :     - create {question}    : Create a new poll with the given question.",
// " :     - vote   {poll_id}     : Vote for a poll option.",                   

#define JOKES   {                                                                                                       \
                    "Why don't scientists trust atoms? Because they make up everything!",                               \
                    "Why did the scarecrow win an award? Because he was outstanding in his field.",                     \
                    "Why did the bicycle fall over? It was two-tired.",                                                 \
                    "How does a penguin build its house? Igloos it together.",                                          \
                    "What do you get when you cross a snowman and a vampire? Frostbite.",                               \
                    "Why don't skeletons fight each other? They don't have the guts.",                                  \
                    "What do you call cheese that isn't yours? Nacho cheese.",                                          \
                    "Why couldn't the leopard play hide and seek? Because he was always spotted.",                      \
                    "What do you get when you cross a joke with a rhetorical question?",                                \
                    "How does a lion greet the other animals in the field? 'Pleased to eat you!'",                      \
                    "Why don't programmers like nature? It has too many bugs.",                                         \
                    "What do you call fake spaghetti? An impasta.",                                                     \
                    "How does a robot eat guacamole? With a micro-chip.",                                               \
                    "Why do cows have hooves instead of feet? Because they lactose.",                                   \
                    "What did the janitor say when he jumped out of the closet? 'Supplies!'",                           \
                    "Why did the golfer bring two pairs of pants? In case he got a hole in one.",                       \
                    "What does a cloud wear under his raincoat? Thunderwear.",                                          \
                    "Why did the tomato turn red? Because it saw the salad dressing!",                                  \
                    "What do you call a factory that makes good products? A satisfactory.",                             \
                    "Why don't some couples go to the gym? Because some relationships don't work out.",                 \
                    "What do you call a bear with no teeth? A gummy bear.",                                             \
                    "What does one wall say to the other? 'I'll meet you at the corner.'",                              \
                    "Why was the computer cold? It left its Windows open.",                                             \
                    "What did the left eye say to the right eye? 'Between you and me, something smells.'",              \
                    "Why was the belt arrested? For holding up a pair of pants.",                                       \
                    "What did the ocean say to the beach? Nothing, it just waved.",                                     \
                    "How do you catch a squirrel? Climb a tree and act like a nut!",                                    \
                    "What did one hat say to the other? 'You stay here, I'll go on ahead!'",                            \
                    "Why did the golfer bring extra socks? In case he got a hole in one.",                              \
                    "What did the big flower say to the little flower? 'Hey, bud!'",                                    \
                    "How do you organize a space party? You planet.",                                                   \
                    "What do you call a fish with no eyes? Fsh.",                                                       \
                    "Why did the cookie go to the hospital? Because it felt crummy.",                                   \
                    "What do you call an alligator in a vest? An investigator.",                                        \
                    "Why did the scarecrow become a successful neurosurgeon? Because he was outstanding in his field.", \
                    "What do you call a bear that’s stuck in the rain? A drizzly bear.",                                \
                    "What do you get when you cross a snowman and a dog? Frostbite.",                                   \
                    "How do you make a tissue dance? Put a little boogie in it.",                                       \
                    "Why did the orange stop? It ran out of juice.",                                                    \
                    "How does a farmer count his cows? With a cow-culator.",                                            \
                    "What did one plate say to the other plate? 'Lunch is on me.'",                                     \
                    "Why did the bicycle fall over? It was two-tired.",                                                 \
                    "What do you get when you cross a dinosaur with a pig? Jurassic Pork.",                             \
                    "What do you call a pile of cats? A meow-tain.",                                                    \
                    "How do you make holy water? You boil the hell out of it.",                                         \
                    "Why did the picture go to jail? Because it was framed.",                                           \
                    "What did the grape do when it got stepped on? Nothing but let out a little wine.",                 \
                    "What did the janitor say when he jumped out of the closet? 'Supplies!'",                           \
                    "Why did the scarecrow win an award? Because he was outstanding in his field.",                     \
                    "What kind of music do mummies listen to? Wrap music.",                                             \
                    "How do you catch a squirrel? Climb a tree and act like a nut.",                                    \
                    "What did one wall say to the other wall? 'I'll meet you at the corner.'",                          \
                    "How does a penguin build its house? Igloos it together.",                                          \
                    "Why did the golfer bring two pairs of pants? In case he got a hole in one.",                       \
                    "Why don't you ever see elephants hiding in trees? Because they're so good at it.",                 \
                    "Why don't skeletons fight each other? They don't have the guts.",                                  \
                    "What do you call a can opener that doesn’t work? A can’t opener.",                                 \
                    "Why did the tomato turn red? Because it saw the salad dressing!",                                  \
                    "What did the ocean say to the shore? Nothing, it just waved.",                                     \
                    "Why do cows wear bells? Because their horns don't work.",                                          \
                    "How does a lion greet the other animals in the field? 'Pleased to eat you!'",                      \
                    "Why did the math book look sad? Because it had too many problems.",                                \
                    "Why did the cookie go to the hospital? Because it felt crummy.",                                   \
                    "What’s a skeleton’s least favorite room in the house? The living room.",                           \
                    "How do you make a tissue dance? You put a little boogie in it.",                                   \
                    "What do you call a fish wearing a bowtie? Sofishticated.",                                         \
                    "Why did the bicycle fall over? It was two-tired.",                                                 \
                    "How does a farmer count his cows? With a cow-culator.",                                            \
                    "Why don’t some couples go to the gym? Because some relationships don’t work out.",                 \
                    "How does a penguin build its house? Igloos it together.",                                          \
                    "What did one hat say to the other? 'You stay here, I'll go on ahead!'",                            \
                    "Why did the golfer bring two pairs of pants? In case he got a hole in one.",                       \
                    "How do you organize a space party? You planet.",                                                   \
                    "What did one wall say to the other? 'I'll meet you at the corner.'",                               \
                    "What did the grape do when it got stepped on? Nothing but let out a little wine.",                 \
                    "What do you call cheese that isn’t yours? Nacho cheese.",                                          \
                    "Why did the math book look sad? It had too many problems.",                                        \
                    "How do you make holy water? You boil the hell out of it.",                                         \
                    "What do you call a bear with no teeth? A gummy bear.",                                             \
                    "What did the big flower say to the little flower? 'Hey, bud!'",                                    \
                    "How do you make a tissue dance? Put a little boogie in it."                                        \
                }

// weather command:
#define HOST "api.openweathermap.org"
#define PORT 80
#define KEY "010b99d131be7191865d4110f87f7db3"

// Error codes:
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464


//NON FATAL ERRORS FOR THE BOT
#define __NONFATAL(output) (output == "PING" || output == "ping")

//FATAL ERRORS FOR THE BOT
#define __FATAL(CODE) (CODE == ERR_NONICKNAMEGIVEN \
            || CODE == ERR_ERRONEUSNICKNAME \
            || CODE == ERR_NICKNAMEINUSE \
            || CODE == ERR_NICKCOLLISION \
            || CODE == ERR_NEEDMOREPARAMS \
            || CODE == ERR_ALREADYREGISTERED \
            || CODE == ERR_PASSWDMISMATCH)


#define __LOG(output, color) std::cout << color << output << RESET << std::endl;
#define __GETARGET (fields[2] == BOT) ? fields[0] : fields[2]


// ERRORS:
#define SYNXERROR " :Error use /help to see the availble commands."
#define SUDOERROR " :U are not one of my masters, I cant help you."

#endif
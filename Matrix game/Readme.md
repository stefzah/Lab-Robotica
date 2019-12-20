# P O N G

[Video](https://www.youtube.com/watch?v=TtWTztKXtfY)

As the title suggests, my matrix game is called P O N G.

**Description of the project**: The purpose of the game is to hit the ball and send it to the enemy side. If the enemy isn't fast enough to hit the ball again, the other player gets 1 point. The game stops when one of the players gets to 3 points.

**Hardware**:

- Game menu is displayed on an LCD display.
- The game is displayed on an 8x8 LED Matrix driven by MAX7219 driver.
- The joystick is used for navigating in the game menu.
- The buttons are used for moving the 'walls' up and down. There are four buttons in case of a PvP match. 
- The potentiometer is used for adjusting the lcd screen contrast.

**About the game**
- Game Modes: You can play this game against the bot, or against another friend(or enemy in our case). 
- Score: Every time the ball gets past a player wall, the other player gets 1 point.
- Levels : The game it has 3 levels: easy, medium and hard. For each level, the speed of the bot is increased 
- Progress: Whenever the ball hits a wall, the speed of the ball increases, making it harder for both players to hit it.
- End Game: The player who won is congratulated on the lcd display, bathing him in glory.

**Menu**
- Start: You'll get to choose to play against the BOT or against a human. Against another human, you get the option to change the name of each player.
- Setup: Here you can change the name of the player and the difficulty of the BOT.
- Info: Here you can see information about the almighty creator of the game. 
- HighScore: The highscore is represented by the player who beated the hardest difficulty. If two players beated the highest difficulty, the one who won in the least amount of rounds is displayed.
- While there is no match going, a "P" is displayed on the matrix ("P" is the first character of P O N G)

# Project Hiawatha Multiplayer
A modified version of Project Hiawatha for multiplayer use.

## Theoretical format
##### (Which we know is nothing like what we will end up with)  
Bold indicates client/server admin action, italics indicate client/server machine process/function  

### Sever Side  
1. <b>Select map </b> -> <i>set number of civs</i>  
2. <b>Select civ to be Human, AI, Either, or Empty</b>  
3. <b>Start server</b> -> <i>Await connections (1 [2?] - max human fillable civs)</i>  
4. On connection -> <i>Add Player name to list; end connection if name already is being used (Need return message)</i>  
5. On data recieved -> <i>Store needed info for civ init. Send update to other clients to reflect selection</i>  
6. On all players ready -> <i>Start 10 second count down (Can be canceled by player unready-ing)</i>  
7. On timeout (not canceled) -> <i>Init Game</i>  

### Client Side
1. <b>Select Multiplayer</b>  
2. <b>Enter server IP and desired nickname</b> -> <b>Press "Connect"</b>
3. <b>Change nickname if connection refused, else select desired civ</b> -> <b>Press "Ready"</b>  
4. <b>Wait</b>

## General Roles
##### (Once again, probably nothing like what we will end up with)

### Server
+ Map Generation  
+ AI Logic Processing  
+ End Game  
+ Turn Controller  
+ Data routing (for when clients send updates)

### Client
+ Parse Server data
  + This will be mostly data updates to civ objects (which therefore include units and cities)
	+ The Renderer will be automatically run at the start of each turn after recieving and processing data from the server
+ Send updated information to server upon 'end turn' phase

## Other notes
* All turns will be sequential (none of that simultaneous magic that Civ V uses)
* Client and Server will need to be stripped down versions of P.H.
  * The server won't need input handling, rendering, and other GUI control
	* The client won't need AI Processing or turn control, but will need some of the more "realtime" functionality such as reorganizing citizens when a city's focus is changed.
* Don't even think about save/load

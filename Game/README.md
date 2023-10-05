<h1>Folder content</h1>
<p>
  You can see in this folder some sources written in C++ and PDDL languages. There is also a file representing a bash script <b>compile.sh</b>. This script just
  helps to compile new or modified project. Some of the values in this .sh file you need to change to successfully work with project.
</p>

<p>
  <b>Fonts</b> folder contains a font file for giving a text tips during a game process.
</p>

<p>
  <b>Images</b> and <b>Textures</b> folders contains raster images.
</p>

<p>
  In <b>PDDL</b> directory is located a constant PDDL Domain file. Besides, during a game in this folder generates a PDDL problem file. It may be changed by program.
</p>

<p>
  <b>Include</b> folder consists of three files: one header file <i>.h</i> and two it's realizations in form of <i>.cpp</i> files. In these files is described a pair of C++ classes:
  <i>Game</i> class and <i>Planner</i> class that includes a Game object. Game class is engaged in generating SFML window content: creating person cleaner,
  cells field, cobblestones and trash. At first, there are random size and coordinates of objects. And then, during plan execution, Game class displays movement of the cleaner
  and cobblestones disappearing. Planner class interacts with PLanSys2 planner: it sends request with problem file and domain file and gets a reply with specific plan steps. Received plan
  Planner class processes and says to Game to display its steps (actions).
</p>

<p>
  File <b>game.cpp</b> contains C++ <i>main()</i> function.
</p>

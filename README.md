<h1>PDDL-Game-For-PlanSys2-Planner</h1>
<p>C++ SFML visualization of the PDDL PlanSys2 planner</p>
<h2>PDDL</h2>
<p>
 <b>PDDL</b> (Planning Domain Definition Language) is a language that allows to generate different <b>plans</b> by automatic systems. It dates back to 1998 when professor Drew McDermott and his colleagues decided to create a common standard language which will be main tool of the International Planning Competition (IPC; now it's named as <b>ICAPS</b>). The result of the implementation of their intention cannot be overestimated! This development was called "PDDL".
</p>

<p>
 PDDL assumes that developer create a <b>planning doman</b> in one .pddl file and <b>task</b> or <b>problem</b> for the planner in another. In the first file programmer should include his
 view of <b>subject area</b> the planner will work with. In the second — a problem which planner has to solve. That repository in particular involves an arsenal of popular PDDL version <b>2.1</b>.
 This version includes such novations as: numeric fluents, plan-metrics and durative/continuous actions. Read more about this in your preferred resources or visit my website: <a href="http://matesspace.h1n.ru/articles/ros/ros0/ros0.php">http://matesspace.h1n.ru</a> (I'm from Russia :).
</p>

<h2>Subject of the repository</h2>
<p>
 This repository you're now in demontrates PDDL cooperation with C++ libraries and tools. This project uses SFML
 library (<a href="https://www.sfml-dev.org/">https://www.sfml-dev.org/</a>) for visualization of PDDL plan execution. Operators (or actions) of plans are generated by <b>PlanSys2</b> planner which has to be installed into <b>ROS2</b> system on Linux <b>Ubuntu 22.04</b>, for example. Main function in <i>game.cpp</i> file calls planner via Linux bash interpreter (though &lt;cstdlib&gt; <i>system</i> command).
</p>

<p>
 Though temp files (which are always have been removed after program closing) a problem generating by <i>Planner::getPDDLProblem()</i> method and source doman in <i>PDDL/game_domain.pddl</i> file
 are sent to planner input. The PlanSys2 planner send a reply to C++ program via temp file containing a new plan. This file is read by <i>Planner::getPDDLPlan()</i> method and separate elements of this plan are created (C-structure objects that form a singly linked list). Every such element is simple action (operator) described in domain file that has to be executed by C++ SFML visualization.
</p>

<p>
 Then <i>Game</i> class just draws into SFML window cleaner's steps and stones' removing.
</p>

<h2>Demonstrating</h2>
<p>
 ![image](https://github.com/LifeSweetener/PDDL-Game-For-PlanSys2-Planner/assets/80912103/8b1d4862-1d54-41b6-afe7-3811727971c2)
 ![image](https://github.com/LifeSweetener/PDDL-Game-For-PlanSys2-Planner/assets/80912103/3920b7d1-1c14-4675-acf6-e0c7cd171e1d)
 ![image](https://github.com/LifeSweetener/PDDL-Game-For-PlanSys2-Planner/assets/80912103/e8687c81-f395-43c9-bafd-1a0948177aa3)
 ![image](https://github.com/LifeSweetener/PDDL-Game-For-PlanSys2-Planner/assets/80912103/68ed1644-7887-4899-94f1-7913f6947949)
</p>

<h2>Installation</h2>
<p>
 To install this project on the computer it's necessary to download and install <b>ROS2</b> and <b>PlanSys2</b> on preffered operating system.
 After that it's needful to make sure that the <b>g++</b> compiler is available on target system. Plus, <b>SFML</b> library is also required to be installed.
 This project use both <b>header</b> files of SFML library (.hpp) (to compile the project) and shared libraries of SFML <i>lib</i> directory (<b>.so</b> on Unix and <b>.dll</b> on Windows)
 (to successfully link all object files in this project).
</p>

<p><b>Thanks for your attention and have a nice time! :)</b></p>

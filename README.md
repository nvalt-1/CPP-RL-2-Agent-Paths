Rewrite of an old class assignment. 
Has not been tested on windows or mac.

TODO:
- Need to make a gui (using command line for now)
- Need to allow for multiple experiments (hard-coded in main function)
- Project structure needs to be organized
- Add make files
- Allow arbitrary experiments

to compile manually:  
`cd src`  
`g++ -c position.cpp worldconfig.cpp rlstate.cpp qtable.cpp agent.cpp world.cpp clidisplay.cpp pdworld.cpp`  
`g++ position.o worldconfig.o rlstate.o qtable.o agent.o world.o clidisplay.o pdworld.o -o pdworld`  

Specification:
=============

COSC4368-RL-Project

In this project we will use reinforcement to learn and adapt “promising paths” in 2-agent setting. Learning objectives of the 2022 COSC 4368 Group Project include:
* Understanding basic reinforcement learning concepts such as utilities, policies, learning rates, discount rates and their interactions.
* Obtain experience in designing agent-based systems that explore and learn in initially unknown environment and which are capable to adapt to changes. 
*	Learning how to conduct experiments that evaluate the performance of reinforcement learning systems and learning to interpret such results. 
*	Development of visualization techniques summarizing how the agents move, how the world and the q-table changes, and the system performance. 
*	Development of path visualization and analysis techniques to interpret and evaluate the behavior of agent-based path-learning systems.
*	Develop and learn coordination strategies for collaborating agents
*	Learning to develop AI software in a team

In particular in this project you will use Q-learning/SARSA  for the PD-Word assuming a 2 agent setting, conducting four experiments using different parameters and policies, and summarize and interpret the experimental results. Moreover, you will develop path visualization techniques that are capable to shed light on what paths the learning system actually has learnt from obtained Q-Tables—we call such paths attractive paths in the remainder of this document. Moreover, you will analyze if the two agents collaborated well by avoiding blockage that occurs if the two agents work on the same path.

Two agent named ‘M’ (male) and ‘F (female) are solving the block transportation problem jointly. Agent alternate applying operators to the PD-World, with the female agent acting first. Moreover, both agents cannot be in the same position at the same time; consequently, there is a blockage problem, limiting agent mobility and ultimately efficiency in case that both agents work on the same path at the same time.  There are two approaches to choose from to implement 2-agent reinforcement learning.
- Each agent uses his/her own reinforcement learning strategy and Q-Table. However, we assume that the position the other agent occupies is visible to each agent, and can therefore can be part of the chosen reinforcement learning state space. 
- A single reinforcement learning strategy and Q-Table is used which moves both agents, selecting an operator for each agent and then executing the selected two operators.
Extra credit is given to groups who devise and implement both 2-agent learning approaches and compare their results for experiments 2 and 3 (see below) 

In experiments we assume that q values are initialized with 0 at the beginning of the experiment. The following 3 policies will be used in the experiments:

* PRANDOM: If pickup and dropoff is applicable, choose this operator; otherwise, choose an applicable operator randomly.
*	PEXPLOIT: If pickup and dropoff is applicable, choose this operator; otherwise, apply the applicable operator with the   highest q-value (break ties by rolling a dice for operators with the same q-value) with probability 0.8 and choose a different applicable operator randomly with probability 0.2. 
*	PGREEDY: If pickup and dropoff is applicable, choose this operator; otherwise, apply the applicable operator with the highest q-value (break ties by rolling a dice for operators with the same q-value).

Objectives of the Experimental Evaluation: Besides analyzing the performance of various variations with respect to the bank account/how quickly the transportation problem was solved, the experimental evaluation should also additionally analyze:
- Agent coordination: Do the two agents get in their ways blocking each other or do they do a good job in dividing the transportation task intelligently between each other. Agent coordination could, for example, be measured by computing the average Manhattan distance between the two agents during the run of a specific experiment.
- Paths learned: Does the particular approach do a good job in learning paths between block sources and block destinations; is the learnt path the shortest path or close to the shortest path between the source and the destination. 

Please conduct the following four experiments:
1.	In Experiment 1 you use alpha=0.3 and gamma=0.5, and run the traditional Q-learning algorithm for 8000 steps; initially you run the policy PRANDOM for 500 steps, then
  - a)	Continue running PRANDOM for 7500 more steps 
  - b)	Run PGREEDY for the remaining 7500 steps
  - c)	Run PEXPLOIT for the remaining 7500 steps
Summarize and interpret the different results you obtain by running these three strategies. Also report one of the final Q-Table of experiment 1.c. Also assess the quality of the coordination between the two agents for experiments 1b and 1c.
2.	Experiment 2 is the same as experiment 1.c except you run the SARSA q-learning variation for 8000 steps. When analyzing Experiment 2 center on comparing the performance of Q-learning and SARSA. Also report one of the final Q-tables of this experiment.  Also assess the quality of agent coordination, 
3.	In Experiment 3 you rerun either  Experiment 1.c or 2 but with learning rates alpha=0.15 and alpha=0.45.  When interpreting the results focus on analyzing the effects of using the 3 different learning rates on the system performance. 
4.	Experiment 4 is the somewhat similar to Experiment 1c or 2; you use alpha=0.3 and gamma=0.5 in conjunction with either Q-learning or SARSA  as follows: you run PRANDOM for the first 500 steps; next, you run PEXPLOIT; however, after a terminal state is reached the third time, change the two pickup locations to: (1,2) and (4,5); the drop off locations and the Q-table remain unchanged; finally, you continue running PEXPLOIT with the “new” pickup locations until the agent reaches a terminal state the sixth time. When interpreting the results of this experiment center on analyzing on how well the learning strategy was able to adapt to the change of the pickup locations and to which extend it was able to learn “new” paths and unlearn “old” paths which became obsolete. 

For all experiments, if a terminal state is reached, restart the experiment by resetting the PD world to the initial state, but do not reset the Q-table. Run each experiment twice, and report  and interpret the results; e.g., utilities computed, rewards obtained in various stages of each experiment

Assess which experiment obtained the best results. Next, analyze the various q-tables you created and try identify attractive paths  in the obtained q-tables, if there are any. Moreover, briefly assess if your system gets better after it solved a few PD-world problems—reached the terminal state at least once. Briefly analyze to which extend the results of the two different runs agree and disagree in the 4 experiments. Analyze agent coordination for experiments 1.c and 4. Finally, analyze how well the approach adapted to change in the fourth experiment. 

Moreover,
*	Make sure that you use different random generator seeds in different runs of the same experiment to obtain different results—having identical results for the 2 runs of the same experiment is unacceptable. It is okay just to report and interpret the Q-tables for the better of the two runs for each experiment, but you should report the performance variables for all eight runs.  
*	You should use the traditional Q-learning and SARSA algorithm in the project and not any other Q-learning variations or reinforcement learning algorithms!
*	Never update the q-values of operators are not applicable in a Q-Tables!
*	Groups who develop good methods for visualizing q-tables and good visualizations for the analysis of attractive paths obtain extra credit. 
*	Groups who develop sophisticated methods to analyze agent coordination receive a small amount of extra credit. 
*	Allow in your software design that the positions of dropoff and pickup positions might be changing before and during a run; otherwise, you will need to write a lot of additional software for experiment4.
*	Evidence of the running of your system has to be provided using screen shots that will be delivered in a separate document.  
*	Groups that develop a very well designed and visually appealing visualization component will receive extra credit for this part of the 4368 Group Project!

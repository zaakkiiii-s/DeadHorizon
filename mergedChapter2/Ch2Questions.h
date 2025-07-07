#pragma once
#include <vector>
#include <string>

using namespace std;
#ifndef CH2QUESTIONS_H
#define CH2QUESTIONS_H

using namespace std;

namespace Chapter2 {
	struct Question {
		string text;
		vector<string> options;
		vector<int> outcomes;
		vector<string> nextQuestions;
		vector<string> prompts;
	};

	//check if main and pathway questions are all wrong: 
	//Q2: 1 main, 2 pathway
	//Q4: 1 main, 2 pathway
	//Q5: 1 main, 2 pathway
	//Q6: 1 main, 2 pathway
	//Q7: 1 main, 2 pathway


	namespace Questions {
		const vector<Question> mainQuestions = {
			{    //Question, Options, Points, Responses
				//zombie spam before question 1 
				 "A distant groan echoes through the empty streets, sending shivers down your spine. You freeze. A horde of zombies lurches just around the corner. Sarah`s eyes widen. \"We need to be careful. What do we do?\"",
				{"Stay hidden", "Investigate the noise", "Run away"},
				{0,-1,0},
				{"2", "1-B", "2"},
				{
					"You hold your breath as the horde shambles past, oblivious to your presence.",
					"You step on broken glass. A zombie turns its head. \"RUN!\" Sarah shouts.",
					"You escape but alert zombies to your presence.", 
				}


			},
			{
				"Panting, you and Sarah finally stop outside an abandoned pharmacy. The glass doors are cracked but intact. Inside, overturned shelves and scattered pill bottles hint at past looters. \"Jacob needs medicine, \" Sarah says. \"but we have to be careful. How do we get in?\"" ,
				{"Look for another entrance", "Break the window", "Kick the door down"},
				{0,-1,0},
				{"3", "2-B", "3"},
				{
					"You find a side door, slipping inside undetected.",
					"The glass shatters, the sound echoing down the street. Moans rise in response.",
					"The door gives way, but the noise draws unwanted attention."
				}



			},
			{
				"Inside, the pharmacy is a mess of broken glass and empty shelves. You rummage through the remains, your fingers trembling as you find a bottle of painkillers, only to realize they expired years ago. Sarah looks over your shoulder. \"Do we risk it?\" " ,
				{"Take it anyway", "Keep searching", "Grab everything"},
				{-1,0,0},
				{"4", "4", "4"},
				{
					"You pocket the expired pills, hoping they will help later, but using them will be risky.",
					"You dig deeper and find a usable bottle.",
					"You carry extra supplies, but it slows you down."
				}

			},
			{
				"As you step towards the exit, a piercing alarm suddenly blares through the store. The old security system wasn`t completely dead after all. Zombies outside stir at the noise. Sarah curses. \"We need to move, now!\"" ,
				{"Find a back exit", "Hide Inside", "Fight your way out"},
				{0,-1,0},
				{"5", "4-B", "5"},
				{
					"You slip out through an alley just in time.",
					"The zombies break in, trapping you.",
					"You escape but waste precious ammo."
				}

			},
			{   //pick lock 
				//Jacob`s shelter is an old office building, its the doors are tightly closed. 
				// You must pick the lock in order to open 
				// sequence

				"Upon opening the doors, its entrance is blocked by fallen debris. You hear his weak voice within the office building. \"I`m here... but I\`m trapped.\"" ,
				{"Look for another path around the debris", "Move the debris to clear a path", "Call out to Jacob"},
				{0,-1,0},
				{"6", "5-B", "6"},
				{
					"You accidentally break something and zombies hear you.",
					"The noise attracts zombies.",
					"He hears you, but so do nearby zombies."
				}

			},
			{
				"Sarah stumbles beside you, struggling to keep up, her leg infected and swollen. She tries to push you away, refusing to slow you down. \"Go on without me... I`m a burden.\" "  ,
				{"Carry her on your back", "Drag her along", "Stop to make a stretcher from nearby debris"},
				{0,-1,0},
				{"7", "6-B", "7"},
				{
					"You refuse to leave her behind and gently lift her onto your back. It`s slow, but it`s the right choice. Despite the added weight, you`re determined to keep her safe.",
					"Your attempts to drag Sarah cause her pain to worsen. The strain on both of you is unbearable, and her cries make it clear you`ve made the wrong decision.",
					"Sarah begs you to leave, but you scan the area and quickly gather fallen branches and your jacket.\"We`re doing this together,\", you say. With some effort, you build a makeshift stretcher and gently help her onto it.You drag it behind you, slow and tiring, but safer for her. She watches you in quiet awe."
				}

			},
			{
				"Night is falling, and the streets aren`t safe. You need a place to hide. Sarah scans the horizon, eyes wide with fear. \"Where do we go?\" "  ,
				{"An old subway station", "A half-collapsed building"},
				{0,-1},
				{"8", "7-B"},
				{
					"You lead Sarah down to the subway station, finding it mostly undisturbed. The underground provides some shelter from the dangers above. The area seems abandoned, and the thick concrete walls shield you from the roaming undead. It`s not perfect, but for now, it`s safer.",
					"The building looks precarious, and the walls creak ominously. As you enter, part of the ceiling gives way, narrowly missing you both. The place is dangerous, and you`re forced to move quickly, but not without injury."
				}

			},
			{
				"Sarah`s breathing has slowed, but the tension in her eyes is still there. Her leg is getting worse, and time is running out. \"We can`t stay here for too long. We need to move soon, or we risk being surrounded. What do we do?\""   ,
				{"Stay here to give Sarah more time to rest", "Push on immediately, hoping to find somewhere safe", "Rest for a short time and then try to fortify the area before moving"},
				{0,0,0}, //end chapter 2, end of chapter 2, restart
				{"-1","-1","-2"},
				{
					"Sarah needs the rest, but staying could lead to dangerous consequences. The longer you wait, the more the undead will close in, and you risk losing the only safe spot you have.",
					"You know moving now might be risky, but if you don`t act fast, things will get worse. However, pushing Sarah too hard could make her injury worse, and you might attract attention from the zombies.",
					"You try to buy a little more time by reinforcing your position, setting up a makeshift barricade with what you can find. Sarah rests as you work, but as you hammer down a plank of wood, the loud noises echo in the surrounding area. You don`t realize it until it`s too late: the zombies are drawn to the sound. The barricade is nowhere near strong enough to hold them back. Within moments, you`re surrounded. You both die."
				}

			}

		};

		const vector<Question> pathwayQuestions = {
			{      //1-B
				"Your heart pounds as the zombies chase you down the ruined street. \"We need to lose them!\" Sarah yells. The dead are faster than they look.",
				{"Climb a building", "Keep Running"},
				{0, -1},
				{"2", "2"},
				{
					"You scramble up a rusted fire escape, escaping to the rooftops, only descending once the streets fall silent again.",
					"Your lungs burn, and exhaustion sets in. You stumble. A zombie lunges and grazes Sarah`s leg. Harmful, not deadly... at least not yet."
				}

			},
			{   //2-B
				"Dealing with the swarm of zombies, they begin to shuffle toward the store, drawn by the noise. \"They`re coming! What do we do?\" Sarah gasps. ",
				{"Barricade the door", "Hide behind the counter"},
				{0, -1},
				{"3", "2-C"},
				{
					"You push a shelf against the entrance, buying time to escape.",
					"The zombies find you. There`s no way out."
				}

			},
			{   //2-C
				"The zombies close in, their guttural groans echoing through the store. There`s no time to run—only to fight. Sarah grips your arm, eyes wide with fear. \"We have to do something!\" ",
				{"Use your weapon to fight your way out", "Try to push past them"},
				{0, -1},
				{"3", "3"},
				{
					"You take down a few zombies, clearing a path. \"Close call,\" Sarah breathes. \"Let`s move!\"",
					"In the chaos, you make a desperate attempt to push through the zombies, but they`re too many, and you`re too slow. One of the zombies grabs hold of your leg, dragging you to the ground. Sarah screams as you`re overwhelmed by the horde, her voice quickly fading into the sounds of the undead feasting. The last thing you feel is the sharp sting of their teeth sinking into your flesh, your screams are the final thing Sarah hears as you lose consciousness."
				}

			},
			{   //4-B
				"The zombies crash through the front entrance, groaning hungrily. Sarah grips your arm. \"We have to get out of here! Think fast!\" ",
				{"Climb onto the store shelves to stay out of reach", "Make a run for the front door"},
				{0, -1},
				{"5", "4-C"},
				{
					"The zombies struggle below as you leap from one shelf to another, finally reaching an air vent. You and Sarah crawl to safety.",
					"The entrance is overrun. A zombie grabs your leg, pulling you down."
				}

			},
			{   //4-C
				"With the undead closing in, Sarah shouts, \"There`s gotta be another way!\". You spot a fire extinguisher nearby. ",
				{"Use the fire extinguisher to blind the zombies and push through", "Try to barricade yourself in a storage room"},
				{0, -1},
				{"5", "5"},
				{
					"The blast of white fog disorients them, and you and Sarah slip past into the alley.",
					"With the zombies closing in, you quickly drag Sarah into a storage room and barricade the door with crates. But the sound of the undead slamming against the barrier grows louder, and the crates start to shake. Before you can react, the door gives way, and the zombies pour in, swarming over you both. Despite your efforts to push Sarah to safety, you`re quickly overwhelmed, and the last thing you hear is her terrified scream as the darkness consumes you."
				}

			},

			{   //5-B
				"The sound of you clearing a path echoes through the empty streets. Distant groans turn into a growing chorus. Sarah grips your arm.\"They`re coming! We need to hurry!\" ",
				{"Hide in a nearby car until the horde passes", "Attempt to move the debris faster"},
				{0, -1},
				{"6", "5-C"},
				{
					"You and Sarah crouch inside an abandoned car. The zombies shuffle past, and after a few tense minutes, the street is clear again.",
					"The noise only grows louder. Zombies surround you before you can get inside."
				}

			},
			{   //5-C
				"The undead are closing in fast. Jacob is still trapped inside. You and Sarah run outside.  Sarah looks at you, fear in her eyes.\"We have to do something!\"  ",
				{"Use a fire escape ladder to reach the roof", "Try to fight the zombies"},
				{0, -1},
				{"6", "6"},
				{
					"You climb up and find another way inside through a broken skylight. Jacob is safe.",
					"The undead are closing in fast, their growls growing louder as they surround you and Sarah. Jacob`s trapped inside, and the door won’t budge. Sarah’s eyes are wide with fear. \"We have to do something!\" With no other choice, you raise your weapon and make a desperate attempt to fight back. But there are too many of them. The zombies swarm you from all sides, tearing through your defenses. Sarah screams as you’re pulled under, the last thing you feel is the crushing weight of their relentless hunger."
				}

			},
			{   //6-B
				"You`re now faced with the consequences of your earlier choice, and the weight of your actions begins to settle in. Sarah is in worse shape than you expected, and the journey ahead is even more uncertain.",
				{"Focus on getting to safety as quickly as possible", "Stop and rest, hoping to regain strength"},
				{0, -1},
				{"7", "6-C"},
				{
					"Despite everything, you push forward with determination. You find a safer route and continue your journey, doing your best to shield Sarah from further harm. Time is running out, but you’re still fighting.",
					"Stopping to rest only allows more danger to close in. You`re ambushed by a small group of zombies. Despite a quick fight, it takes a toll, and you lose precious time. Sarah`s condition continues to deteriorate."
				}

			} ,
			{   //6-C
				"You ignored Sarah`s worsening condition or stopped to rest, and the infection took a deadly toll on her. She’s barely clinging to life now, her fever high, and her breathing labored. You try to keep going, but the situation has become dire. As you struggle to carry on, you hear the unmistakable growl of zombies closing in on you both. Sarah, delirious from the infection, doesn`t even notice. She mutters weakly, \"Jacob\", as if calling out to someone who`s no longer within reach.",
				{"Use a flare or loud noise to distract the zombies and buy time", "Try to fight the zombies off to buy some time", "Run, leaving Sarah behind to save yourself"},
				{0, -1,-1},
				{"7", "7", "7"},
				{
					"Thinking quickly, you pull out a flare and ignite it, sending a blinding light and loud noise into the night. The zombies turn away momentarily, distracted by the flare. With a burst of energy, you grab Sarah and push forward, stumbling to safety as the threat temporarily fades. You’ve bought yourself some time—but the fight’s far from over.",
					"You draw your weapon and attempt to fight back, but the zombies are too many. They overpower you, and Sarah’s frail body is dragged away from you as you’re swarmed. In your final moments, you hear her cries, fading as the zombies tear into you both. Your vision fades, and everything goes black. You’ve failed.",
					"In a moment of desperation, you turn and flee, abandoning Sarah. But it`s too late. The zombies swarm over her, and she`s lost in seconds. As you run, the sound of the screams echoes behind you, only for the zombies to catch up with you. They drag you down, and you meet the same fate. You`ve failed."
				}

			},
			{   //7-B
				"The building creaks ominously as you and Sarah scramble through the wreckage. The walls are close to giving way, and the air is thick with dust. You try to move quickly, but the structure feels like it could collapse at any moment. ",
				{"Try to find a way out through the back exit", "Look for a place to barricade yourselves and wait for help"},
				{-1, 0},
				{"7-C", "8"},
				{
					"As you rush toward the back exit, part of the building suddenly collapses behind you, trapping you further inside. You turn back, but it`s too late. The walls start crumbling around you. The structure begins to break down even faster, and you know you`re running out of time. You and Sarah are stuck.",
					"You quickly find a sturdy corner and barricade yourselves, hoping to wait out the collapse. The building groans but holds for now. You manage to stay safe long enough for the immediate danger to pass. You both catch your breath and plan your next move."
				}

			},
			{   //7-C
				"You`re trapped beneath a pile of rubble. The air is growing thicker and you`re struggling to breathe. Sarah is moaning in pain beside you.The walls are collapsing. You know you need to make a decision fast.The zombies are already on their way. ",
				{"Try to crawl out from beneath the rubble and fight the zombies", "Look for a place to barricade yourselves and wait for help", "Try to find something to use as a weapon and defend yourself"},
				{-1, 0,-1},
				{"8", "8","8"},
				{
					"As you try to crawl free, the debris shifts, and you`re pinned once again. The zombies find their way into the wreckage, and despite your efforts to fight them off, they overpower you. Sarah’s screams are the last thing you hear as the undead close in. You have died.",
					"You quickly find a sturdy corner and barricade yourselves, hoping to wait out the collapse. The building groans but holds for now. You manage to stay safe long enough for the immediate danger to pass. You both catch your breath and plan your next move.",
					"The zombies are too many, and the debris is too heavy. You try to find a weapon, but the rubble makes it impossible to get anything useful. The zombies reach you both, and with nowhere to escape, they finish what the building started. You and Sarah die in the dark, helpless."
				}

			}



		};

	}
}

#endif
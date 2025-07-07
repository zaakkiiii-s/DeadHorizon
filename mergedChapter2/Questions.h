#include <vector>
#include <string>

using namespace std;
#ifndef QUESTIONS_H
#define QUESTIONS_H



using namespace std;

namespace ZombieGame {


	struct Question {
		string text;
		vector<string> options;
		vector<int> outcomes;
		vector<string> nextQuestions;
		vector<string> prompts;
	};

	namespace Questions {
		const vector<Question> mainQuestions = {
			{    //Question, Options, Points, Responses
				 "First things first, tell me - what`s the best way to kill a zombie?",
				{"Destroy the brain", "Stab it in the heart", "Cut off its arms"},
				{2,0,0},
				{"2", "1-B", "1-C"},
				{
					"Cole nods. \"That`s right. You`ll need to remember that.\" He hands you a combat knife.",
					"\"That won`t stop them,\" Cole sighs. \"Come with me, I`ll show you why.\" You’re forced into a zombie simulation where your mistake nearly costs you your life.",
					"\"That`ll only slow them down.\" Cole shakes his head. \"Let`s see how you handle one up close.\" You are thrown into a fight simulation."
				}

			},
			{
				"Stealth is key. Which of these is best for silent kills?" ,
				{"Bow and Arrow", "Shotgun", "Crowbar"},
				{2,0,1},
				{"3", "2-B", "3"},
				{
					"You land a perfect headshot. Cole nods. \"Good. You`ll need that skill.\"",
					"The explosion echoes, drawing imaginary zombies in the simulation.",
					"It works, but it`s noisy."
				}
			},
			{
				"You`re out scavenging. You find some canned food, but it`s dented. What do you do?" ,
				{"Eat it anyway", "Check for rust or bulging", "Give it to someone else"},
				{0,2,0},
				{"3-B", "4", "3-B"},
				{
					"It`s spoiled, and you get food poisoning.",
					"\"Smart. If it looks bad, don`t eat it.\"",
					"You risk poisoning an ally."

				}
			},
			{
				"You find a river. Do you drink the water?" ,
				{"Yes, it looks clean ", "Boil it first", "Use a filter"},
				{0,2,2},
				{"4-B", "5", "5"},
				{
					"You get sick from bacteria.",
					"\"Good. Always purify your water.\"",
					"\"That will work, but boiling it is safer.\""
				}
			},
			{
				"Someone in your group is badly wounded. What do you do first?" ,
				{"Stop the bleeding", "Give them painkillers", "Clean the wound later"},
				{2,0,0},
				{"6", "6", "6"},
				{
					"\"Right. Keep them from losing too much blood.\"",
					"Doesn`t fix the problem, your ally bled to death.",
					"Infection spreads fast."
				}
			},
			{
				"You need a place to rest. Where do you sleep?"  ,
				{"In an abandoned house", "Outside under the stars", "In a car"},
				{2,0,1},
				{"7", "7", "7"},
				{
					"\"Barricade the doors and you`ll be safe.\"",
					"You get attacked overnight.",
					"\"Not ideal, but it`ll do in a pinch.\""
				}
			},
			{
				"A stranger asks for help. Do you trust them?"  ,
				{"Yes, let them join", "Test them first ", "Refuse immediately"},
				{0,2,1},
				{"8", "8", "8"},
				{
					"They betray you and steal all your supplies",
					"\"Smart. In a world like ours, trust must be earned.\"",
					"\"Not everyone is bad, but caution is good.\""
				}
			},
			{
				"One last thing, kid. When all else fails, what`s the key to survival?"   ,
				{"Strength ", "Intelligence", "Luck"},
				{0,2,0},
				{"", "", ""},
				{
					"\"That helps, but smarts keep you alive.\"",
					"\"Exactly. Stay sharp, stay alive.\"",
					"\"Luck runs out, kid. Smarts don`t.\""
				}
			}

		};

		const vector<Question> pathwayQuestions = {
		   {      //1-B
				"You`re locked in a training room with a caged zombie. It snarls and lunges as Cole watches. \"You`ve got one shot, kid. Where do you aim?\"",
				{"The legs", "The head", "The chest"},
				{-1, 1, -1},
				{"2", "2", "2"},
				{
					"The zombie crawls toward you, forcing you to fight it off.",
					"You land a clean strike, killing it instantly. Cole nods. \"Maybe you got what it takes.\"",
					"The zombie keeps coming! You panic and barely escape."
				}
			},
			{   //1-C
				"\"That was messy,\" Cole sighs. \"What do you do when a zombie grabs you?\" ",
				{"Try to overpower it", "Use your knife to stab the head"},
				{-1, 1},
				{"2", "2"},
				{
					"You waste too much energy. You get bitten in the simulation.",
					"\"Good. Always have a weapon ready.\""
				}
			},
			{   //2-B
				"\"You just attracted a horde. What do you do?\"",
				{"Hide immediately", "Try to shoot them all", "Run away blindly"},
				{1, -1, -1},
				{"3", "3", "3"},
				{
					"The zombies pass by without noticing you.",
					"You run out of ammo, getting overwhelmed, you try to run away but you trip and fall.",
					"Your heavy footsteps attract more zombies, you are unable to escape."
				}
			},
			{   //3-B
				"You`re feeling sick. What`s your best course of action?",
				{"Keep moving despite the pain", "Rest and drink clean water"},
				{-1, 1},
				{"4", "4"},
				{
					"You collapse from exhaustion.",
					"You recover after a few hours."
				}
			},
			{   //4-B
				"You drank unsafe water. Now what?",
				{"Hope for the best", "Find medicine or rest"},
				{-1, 1},
				{"5", "5"} ,
				{
					"You become dehydrated from sickness.",
					"You recover after a rough night."
				}
			}


		};

	}

}

#endif

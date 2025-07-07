
#include <vector>
#include <string>

using namespace std;
#ifndef CH3QUESTIONS_H
#define CH3QUESTIONS_H



using namespace std;
//way to org code. everything insdie this belongs to ch3
namespace Chapter3 {

	//define a struct called question
	//each question contains
	struct Question {
		string text;//contatins narrative 
		vector<string> options;//vector of string containing the options
		int timelimit;//integer that defines the time limit
		vector<string> nextQuestions;// vector containing the ID of the next question based on option selectedd
		vector<string> prompts;//vector containing the prompts(short description of consequence of option selected)
	};

	namespace Questions {
		const vector<Question> mainQuestions = {
			{    //Prompt 1
				 "The entrance is a mess of fallen beams and shattered glass. Every step crunches against debris. The air smells of mildew and something... rotten. A noise. Faint but distinct. A clicking sound, like metal shifting against stone. You freeze. Sarah whispers. \"What was that? \"How do you proceed?",
				{"Crouch low and feel the ground ahead","Use a flashflight carefully", "Run through quickly"},
				25,
				{"1-B","2", "1-B"},
				{
					"You drop into a crouch, the broken floor groaning under your weight. Sarah crouches beside you, barely breathing.",
					"You shine your light just in time, spotting the tripwire. You step over it carefully.",
					"You trip, yanking the wire. A deafening snap fills the air as a hidden trap activates."

				}

			},
			{   //Prompt 2
				"You press forward, every muscle tense. The air grows heavier. A foul stench clogs your nostrils. \"We have to hurry,\" Sarah urges. A muffled groan echoes from a nearby room. Jacob. You rush to the door. It`s blocked by fallen debris. He`s trapped. How do you get to him?" ,
				{"Kick the door down","Use one of your tools to pry the door open", "Move debris carefully"},
				25,
				{"2-B","3", "3"},
				{
					"The noise echoes through the halls. Something stirs in the darkness.",
					"You sling your bag over your shoulder, the coldbar`s cold grip bites into your palms.",
					"You clear a path without making too much noise."
				}
			},
			{   //Prompt 3
				"You managed to safely get to Jacob. Jacob is slumped against the wall, his face pale, his breaths shallow. Blood soaks his pants leg with deep gashes from a close encounter. \"We need to move,\" Sarah mutters, glancing anxiously at the hallway. \"But if we don`t treat that wound, he won`t last long.\" What do you do first?" ,
				{"Treat him later when we get back to headquarters", "Clean the wound and bandage it", "Give Jacob painkillers and help him up"},
				30,
				{"3-B", "4", "3-C"},
				{
					"Jacob is too weak to move quickly, slowing you down. The undead are gaining.",
					"Stops the bleeding, giving him a chance to escape with you and receive proper treatment later." ,
					"You dig into your pack and pull out a small vial of painkillers, pressing two tablets into his trembling hand. "

				}
			},
			{   //4 
				"Jacob stumbles, his eyes fluttering open and shut. His breaths are shallow, his body drenched in sweat. Jacob collapses. His head rolls forward, dangerously close to unconsciousness. If he passes out now, there`s no telling if he'll wake up again. You need to act fast! What do you do?" ,
				{"Engage in conversation with him to keep him awake", "Don`t check on him until we get to headquarters", "Offer him water, and splash some on his face to keep him awake"},
				25,
				{"5", "4-B", "5"},
				{
					"Talking to him stimulates his brain and helps him stay conscious, but your voices could attract unwanted attention.",
					"Jacob slips into unconsciousness. His condition worsens, and now you`re carrying dead weight.",
					"It keeps him awake without making too much noise."
				}
			},
			{   //5
				"You mange to escape safely, into the forest, your breath ragged, and Jacob still barely able to walk. The forest provides cover, but it`s not safe here either. Sarah looks at you, her expression tense. \"We need to get further in. The closer we are to the open field, the easier they`ll spot us.\" What do you do? " ,
				{"Stay close to the forest edge to avoid getting lost", "Head deeper into the forest for more cover", "Follow an overgrown trail to the side, hoping it leads to safety"},
				20,
				{"5-B", "6", "5-C"},
				{
					"You`re spotted by a roaming group of undead. They corner you, and you barely escape.",
					"You find a dense thicket where the undead can`t easily track you.",
					"Infection spreads fast."
				}
			},
			{   //6
				"As dawn breaks, the forest seems strangely calm. Sarah notices it first: a hidden perimeter fence covered in camo netting. A symbol is stenciled on the gate: a corporate logo: VirexCorp. You remember hearing rumors before the outbreak. What do you do? "  ,
				{"Enter the facility cautiously, hoping for answers", "Turn back immediately and find another route", "Stay hidden and observe the facility first"},
				20,
				{"7", "6-B", "6-C"},
				{
					"The stranger sees you as a threat and fires a warning shot. You drop your weapons. Lucky for you, the stanger is no danger",
					"Just as you are about to leave, you catch sight of a man carrying a briefcase entering VirexCorp. By the number of guards surrounding him, it`s obvious the briefcase contains something important.",
					"\"Not ideal, but it`ll do in a pinch.\""
				}
			},
			{   //7
				"You made it safely into the lab. The lab is in ruins, files scattered on the floor show experiments not on curing, but controlling the virus. Alarms blare. A countdown begins : 5 minutes to security lockdown. Sarah bursts in. \"I found something: a vial. It might be the real cure. Or just another experiment.\" What do you do? Do you... "  ,
				{"Take the vial and escape before lockdown", "Destroy all research, including the possible cure", "Stay and upload everything to the public"},
				20,
				{"8", "7-B", "7-C"},
				{
					"After resting, you regain your strength and come up with a solid escape plan.",
					"As you attempt to leave, you’re ambushed by another group of survivors who take your supplies",
					"\"Not everyone is bad, but caution is good.\""
				}
			},
			{   //8
				"You sprint through the building, Sarah by your side. At the exit, she looks at you. What now? Jacob smiles weakly. \"Let`s give them hope.\" Your final choice:  "   ,
				{"Deliver the cure to the resistance, hoping it can save lives", "Use the cure, but for a dangerous new beginning"},
				20,
				{"", ""},
				{
					"You deliver the cure to the resistance,\n igniting a spark of hope. With it,\n they prepare to fight for humanity`s future.",
					"Jacob injects the cure, feeling an intense surge of power.\nIt`s not just a cure it`s an evolution.\nSarah watches in shock as you all realize \nthe world will never be the same, \nand neither will you."

				}
			}

		};

		const vector<Question> pathwayQuestions = {
		   {      //1-B
				"A metallic clang rings out, followed by a whirring noise. Your stomach drops. \"You triggered something! MOVE!\" Sarah yells. Behind you, gears grind, and a makeshift spike trap begins to swing down from the ceiling. What do you do? ",
				{"Jump over the tripwire", "Duck and slide under a nearby desk"},
				15,
				{"2", "1-C"},
				{
					"You dive forward just in time, hitting the ground as the spikes slam into the floor behind you.",
					"You hit the ground and slide beneath the desk, heart pounding."
				}
			},
			{   //1-C
				"The air crackles as another mechanism activates. A second trap! Poisoned darts shoot from the walls! Time is running out. Think quick, what do you do? ",
				{"Duck and roll", "Sprint Forward recklessly"},
				12,
				{"2", "@"},
				{
					"You narrowly avoid the darts, feeling them whistle past your ear.",
					"The darts strike your leg and a burning pain spreads instantly."
				}
			},
			{   //2-B
				"The growls grow louder, and the door rattles. There`s no time to think, only act. Quick! Do something!",
				{"Throw an object to distract them", "Try to sneak past them"},
				12,
				{"3", "@"},
				{
					"You push a desk against the entrance, buying precious seconds.",
					"You slip but the zombies hear it. In seconds, you`re surrounded."
				}
			},
			{   //3-B
				"Jacob groans, struggling to keep up. His steps grow sluggish. The infection is spreading. Sarah grips your shoulder. \"He`s not gonna make it at this rate. We need a plan!\" What do you do? ",
				{"Carry Jacob on your back", "Run faster, hoping Jacob gets a rush of adrenaline to keep him going"},
				18,
				{"4", "3-C"},
				{
					"The zombies follow the noise, giving you a chance to escape.",
					"The zombies notice you, and it`s too late to run."
				}
			},
			{   //3-C
				"Jacob stumbles, his vision swimming. A shadow moves at the far end of the hall. More zombies. They`re getting closer. Time is running out! Do something!",
				{"Use a makeshift splint and keep moving", "Stop and wait for him to recover"},
				15,
				{"4", "@"} ,
				{
					"Slower, but you push through. Sarah covers you as you move.",
					"Jacob collapses as the horde descends there`s no time left to escape."
				}
			},
			{   //4-B
				"Jacob coughs out blood. His breathing is weak, and his skin has gone pale. You drop to your knees, shaking him. He doesn`t respond. Sarah kneels beside you, panic creeping into her voice. \"He`s out cold. We can`t just leave him like this! The undead are lurking nearby. You can`t stay here forever.\" What do you do? ",
				{"Try to shake him awake and yell his name", "Check his pulse, then try slapping his face gently"},
				22,
				{"4-C", "5"},
				{
					"You grab Jacob`s shoulders, shouting his name until your throat burns. His eyelids flutter—just as a guttural groan echoes from the shadows. Sarah yanks your arm. \"They heard us! Move!\"",
					"Your fingers press against Jacob`s neck—a pulse, but faint. A light slap snaps his head sideways. He gasps, eyes wild, but before he can speak, Sarah hisses: \"They’re coming! Run!\" You drag him into the treeline as twigs snap behind you."
				}
			},
			{   //4-C
				"You and Sarah just managed to get Jacob to his feet. The undead are closing in, and you need to make a quick decision. The path ahead splits into two directions, one leads toward the old church, the other toward a dense forest. Sarah looks at you, fear in her eyes. Which way do we go? ",
				{"Head toward the old church for shelter", "Take the forest path, using trees as cover"},
				15,
				{"@", "5"},
				{
					"The church doors won`t budge... and the undead are right behind you.",
					"The forest provides cover, and you manage to stay hidden until it`s safe."
				}
			},
			{   //5-B
				"Snarls echo around you. Shadows move between trees. You duck behind a log, Jacob moans quietly. Sarah whispers, \"We need a distraction or we`re done.\" Which disctraction do you pick? ",
				{"Toss a flare deep into the trees", "Hold your ground and prepare to fight"},
				15,
				{"6", "@"},
				{
					"Flare goes into the trees.",
					"You swing. You fight. But there are too many and they`re relentless."
				}
			},
			{   //5-C
				"You push through the overgrown trail, branches whipping your face. It`s quiet... too quiet. Sarah grabs your arm.\"This doesn`t feel right. Are we even going the right way?\" What do you do next? ",
				{"Take a shortcut through a narrow animal trail", "Mark the trail with broken branches in case you need to retrace"},
				20,
				{"@", "6"} ,
				{
					"The trail ends in a ravine, you slip and fall before you even have time to scream.",
					"You trek through the night, the eerie silence pressing in around you."
				}
			} ,
			{   //6-B
				"You turn away from the perimeter fence, making your way through the quiet forest. The woods soon grow tense, and distant growls hint at zombies wandering nearby. The world outside VirexCorp is more dangerous than you thought. Do you... ",
				{"Continue further into the woods", "Set up a shelter and wait until nightfall to investigate VirexCorp"},
				18,
				{"@", "7"},
				{
					"You head back into the woods exhausted only to walk straight into a waiting horde.",
					"Investigate VirexCorp with more caution."
				}
			},
			{   //6-C
				"Hidden by camouflage, you and Sarah watch the perimeter. Guards patrol, unaware of you. A security terminal near the fence offers access to the facility`s systems. Do you... ",
				{"Wait for the guards to be distracted and sneak inside", "Wait until nightfall to investigate the building"},
				20,
				{"@", "7"},
				{
					"You failed to wait for the right moment, and the guards caught sight of you, forcing you into a desperate confrontation.",
					"You find a safe spot as you wait until nightfall."
				}
			},
			{   //7-B
				"Furious at VirexCorp, you destroy the data and vials, ensuring no one else can use them. The alarms blare louder as footsteps approach. The countdown to lockdown continues. Do you... ",
				{"Escape with Sarah through a hidden exit you discovered earlier", "Hold your ground and face whatever comes next, hoping for a final confrontation"},
				20,
				{"8", "@"},
				{
					"The zombies follow the noise, giving you a chance to escape.",
					"You chose to face them, but VirexCorp`s forces overwhelmed you, and the lockdown sealed your fate."
				}
			},
			{   //7-C
				"You upload the files to the global web, revealing VirexCorp`s secrets. The countdown ticks on. The system seems to be fighting back, the upload slowing down, but you manage to finish. Now what? ",
				{"Escape quickly before VirexCorp can respond", "Stay and confront the guards in hope for more answers"},
				15,
				{"8", "@"},
				{
					"The alarms blare as you rush towards the exit, pulling Sarah along with you.",
					"They immediately spot you and open fire, leaving no chance for escape."
				}
			}

		};

	}

}

#endif

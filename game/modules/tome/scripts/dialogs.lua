constant("dialogs", {})

----------------------------------------------------------
-------------------------- Bree --------------------------
----------------------------------------------------------
dialogs.FARLINA_SNOWFOOT = function()
	dialog.start("Farlina")
	dialog.speaker()
	dialog.speak("The seem like the adventurer kind of people, aren't you? I am so desperate, I need any help I can get.\n")
	dialog.reply_cond("start", "I could be, yes, what is the matter?", dialog.on_quest_status(QUEST_BARROWS_KING, QUEST_STATUS_UNTAKEN))
	dialog.reply_cond("return", "I have found your heirloom.", have_object(USE_INVEN, HAVE_ARTIFACT, ART_FARLINA_HEIRLOOM))
	dialog.reply("I must go, sorry!")

	if dialog() == "start" then
		dialog.speaker()
		dialog.speak("I have lost an heirloom very dear to me when I was passing by the Barrows just outside of the borders of Bree, to the west. Some kind of specter approched and as I ran away in panic I dropped it. The specter must have took it now.\n")
		dialog.reply("ok", "#GI will check this out for you.")
		dialog.reply("No, sorry, I'm quite busy currently...")
		if dialog() == "ok" then
			dialog.stop()
			acquire_quest(QUEST_BARROWS_KING)
			return
		end
	elseif dialog() == "return" then
		dialog.speaker()
		dialog.speak("Oh my! Thanks, you are a good soul!\nOh by the way, I have heard the Mayor is looking for experienced adventurers to help with a bandits problem, you should seem him.\n")
		dialog.speak("#B(You have gained 3 more skill points)\n")
		dialog.conclude()

		local inven, item = have_object(USE_INVEN, HAVE_ARTIFACT, ART_FARLINA_HEIRLOOM)
		item_increase(item, -1)
		item_describe(item)
		item_optimize(item)
		player.skill_points = player.skill_points + 3
		player.redraw[FLAG_PR_STUDY] = true
		change_quest_status(QUEST_BARROWS_KING, QUEST_STATUS_FINISHED)
	end

	dialog.stop()
end

dialogs.BREE_MAYOR = function()
	dialog.start("Marlo Brockhouse", true)
	dialog.speaker()
	dialog.speak("Hail to you adventurer!\n")
	dialog.reply_cond("start", "What can I do to be of service?", dialog.on_quest_status(QUEST_THIEVES, QUEST_STATUS_UNTAKEN))
	dialog.reply("I must go, sorry!")
	if dialog() == "start" then
		dialog.speaker()
		dialog.speak("There seems to be some disturbing activity in the abandoned house just south of here. Could you please go in and investigate?\n")
		dialog.reply("ok", "#GI will at once!.")
		dialog.reply("No, sorry, I'm quite busy currently...")
		if dialog() == "ok" then
			dialog.stop()
			acquire_quest(QUEST_THIEVES)
			return
		end
	end
	dialog.stop()
end
dialogs.BREE_MAYOR_END_QUEST_THIEVES = function()
	dialog.start("Marlo Brockhouse")
	dialog.speaker()
	dialog.speak("Hail to you adventurer! #o(he speaks from outside the house)\n")
	dialog.reply("Hi there, I seem to be, well .. stuck, could you help me?")
	dialog()
	dialog.speaker()
	dialog.speak("Yes, we are clearing the rubble as we speak.\n")
	dialog.reply("Great thanks! But I have alarming news. It seems the head of the thieves guild was an agent of Saruman. But why would Saruman do something like that?\n")
	dialog()
	dialog.speaker()
	dialog.speak("I do not know; we must learn that later. But for now I thank you for helping with the thieves. You may use this house as your own if you so desire -- you deserve that much.\n")
	dialog.reply("#GThanks!. (as he speaks you see some light and soon you are able to climb up)")
	dialog()
	dialog.stop()
	change_quest_status(QUEST_THIEVES, QUEST_STATUS_FINISHED)
end

----------------------------------------------------------
------------------------ Lothlorien ----------------------
----------------------------------------------------------
dialogs.GALADRIEL = function()
	dialog.start("Lady Galadriel")
	dialog.speaker()
	dialog.speak("At last you came!\n")
	dialog.reply_cond("start_necro", "Do your require my services, Lady Galadriel?", dialog.on_quest_status(QUEST_NECROMANCER, QUEST_STATUS_UNTAKEN))
	dialog.reply_cond("end_necro", "The Necromancer of Dol Guldur was in fact Sauron in disguise, as we feared.", dialog.on_quest_status(QUEST_NECROMANCER, QUEST_STATUS_COMPLETED))
	dialog.reply_cond("found_ring", "I have found the One Ring. I can sense the evilness of it, trying to lure me into wielding it.", have_object(USE_INVEN, HAVE_ARTIFACT, ART_ONE_RING))
	dialog.reply_cond("ring_destroyed", "The One Ring is no more, I cast the ring into the fiery core of Mount Doom where it slowly melted away.", dialog.on_quest_status(QUEST_ONE_RING, QUEST_STATUS_COMPLETED))
	dialog.reply_cond("sauron_dead", "Sauron, the dark ennemy of Middle Earth, is no more, we may all rest safely now.", dialog.on_quest_status(QUEST_SAURON, QUEST_STATUS_COMPLETED) and dialog.on_quest_status(QUEST_ONE_RING, QUEST_STATUS_FINISHED))
	dialog.reply("I must go, sorry!")

	if dialog() == "start_necro" then
		dialog.speaker()
		dialog.speak("Lately the shadow is growing over the Mirkwood forest. "..
				"A necromancer has taken the old tower of Dol Guldur as his "..
				"new home. This evil cannot be allowed to spread.\n"..
				"Find the tower deep inside mirkwood and kill the necromancer.\n")
		dialog.reply("ok", "#GI will not allow this evil to spread!")
		dialog.reply("Maybe later.")
		if dialog() == "ok" then
			dialog.stop()
			acquire_quest(QUEST_NECROMANCER)
			return
		end
	elseif dialog() == "end_necro" then
		dialog.speaker()
		dialog.speak("So it was Sauron that lurked in Dol Guldur...\n"..
				"The Enemy is growing in power, But his power "..
				"lies in the One Ring, our only hope is that you find it "..
				"and destroy it. I know it will tempt you, but *NEVER* use it "..
				"or it will corrupt you forever.\n")
		dialog.reply("ok", "#GThen the ring must be found! I will keep both eyes open for it.")
		dialog.reply("Maybe later.")
		if dialog() == "ok" then
			dialog.stop()
			change_quest_status(QUEST_NECROMANCER, QUEST_STATUS_FINISHED)
			acquire_quest(QUEST_ONE_RING)
			return
		end
	elseif dialog() == "found_ring" then
		dialog.speaker()
		dialog.speak("So there is still hope for Middle-earth...\n"..
				"You must hurry to the summit of Mount Doom and cast the ring into the great fire "..
				"this is the only way to destroy it.\n")
		dialog.reply("#GI will!")
		dialog()
		dialog.stop()
		return
	elseif dialog() == "ring_destroyed" then
		dialog.speaker()
		dialog.speak("At last we may triumph over Sauron! The light of the Valar is shining on you my friend.\n"..
				"Now the last, and most dangerous, task awaits you."..
				"You must venture deep in Barad Dur to find Sauron and banish him from Middle-earth forever!\n")
		dialog.reply("#GI shall not fail!")
		dialog()
		dialog.stop()
		change_quest_status(QUEST_ONE_RING, QUEST_STATUS_FINISHED)
		return
	elseif dialog() == "sauron_dead" then
		dialog.speaker()
		dialog.speak("Yes I can feel it, his dark glare is no more. I am truly amazed, "..
			     "you surpassed all the expectations, you are our saviour.\n"..
			     "#o(Galadriel and all the people of Caras Galadhon bows down to you)\n")
		dialog.reply("retire", "Thank you, fair lady, but do not bow before me, I only did what was neccesary and now I will go rest for a while, until I am needed again.\n#B(Retire this character, this will permanently end the game, you have won)")
		dialog.reply("Thank you, fair lady, but do not bow before me, I only did what was neccesary and now I am going to see the wild one last time, fare well good Lady.\n#B(Continue playing)")
		if dialog() == "retire" then
			dialog.stop()
			change_quest_status(QUEST_SAURON, QUEST_STATUS_FINISHED)
			do_cmd_suicide()
		else
			dialog.stop()
			change_quest_status(QUEST_SAURON, QUEST_STATUS_FINISHED)
		end
		return
	end
	dialog.stop()
end
----------------------------------------------------------
------------------------ Rivendell -----------------------
----------------------------------------------------------
dialogs.ELROND = function()
	dialog.start("Elrond")
	dialog.speaker()
	dialog.speak("What may the elfes do for you?\n")
	dialog.reply_cond("found_narsil", "Master Elrond, I have found the shards of Narsil.", dialog.on_quest_status(QUEST_NARSIL, QUEST_STATUS_COMPLETED))
	dialog.reply_cond("seen_isengard_fire", "While passing by the Gap of Rohan I saw large amounts of smoke and war shouts coming from the north. From Isengard, something not right is going on I fear.", quest_data(QUEST_NARSIL).isengard_seen and quest(QUEST_ISENGARD_INVESTIGATE).status == QUEST_STATUS_UNTAKEN)
	dialog.reply_cond("isengard_spied", "Master Elrond, I observed the surroundings of Isengard as you told me. At first I could not see anything, but after a while I stumbled across a patrol of orcs, bearing a white hand marking on their shields!", dialog.on_quest_status(QUEST_ISENGARD_INVESTIGATE, QUEST_STATUS_COMPLETED))
	dialog.reply("I must go, sorry!")

	if dialog() == "found_narsil" then
		dialog.speaker()
		dialog.speak("At last some shred of hope in those troubled times. Thanks for your help.\nI may call upon you later my friend, train yourself until then for the tasks I may ask of you should not be that easy, I am afraid to say..\n")
		dialog.reply("#GGood bye master, I shall be ready when you require my help again!")
		dialog()
		dialog.stop()

		change_quest_status(QUEST_NARSIL, QUEST_STATUS_FINISHED)
		message(color.YELLOW_GREEN, "Elrond touches your forehead. You feel stronger, smarter and wiser!")
		message()
		inc_stat(A_STR)
		inc_stat(A_INT)
		inc_stat(A_WIS)
		inc_stat(A_DEX)
		inc_stat(A_CON)
		inc_stat(A_CHR)
		for j = INVEN_INVEN, INVEN_TOTAL - 1 do
			for i = flag_max_key(player.inventory[j]), 1, -1 do
				if player.inventory[j][i] and (player.inventory[j][i].artifact_id == ART_NARSIL_BLADE or player.inventory[j][i].artifact_id == ART_NARSIL_HILT) then
					item_increase(compute_slot(j, i), -99)
					item_optimize(compute_slot(j, i))
				end
			end
		end
		return
	elseif dialog() == "seen_isengard_fire" then
		dialog.speaker()
		dialog.speak("Ah yes, I had other similar reports of strange activities in the surroundings of Isengard. Please go to Isengard and investigate.\n")
		dialog.reply("#GI will at once!")
		dialog()
		dialog.stop()
		acquire_quest(QUEST_ISENGARD_INVESTIGATE)
		return
	elseif dialog() == "isengard_spied" then
		dialog.speaker()
		dialog.speak("The mark of Saruman, as Gandalf suspected! But please continue your story, I feel there is more to tell.\n")
		dialog.speaker("You")
		dialog.speak("Yes indeed, for I did see the patrol, but so did they! I stood no chance against such a group of orcs, I felt my last day had arrived...\n")
		dialog.speaker()
		dialog.speak("Yet you came back, how did you survive my friend?\n")
		dialog.speaker("You")
		dialog.speak("Shortly after they spotted me I heard a very loud, deep, voice coming from behind me, but when I looked I saw only trees.\nBut soon I realized that it was indeed the tree that spoke, and that it was moving, towering with incredible speed and strength toward the group of orcs. He made a short work of them!\nWhen it was over he told me he was an ent and that those orcs were indeed coming from Isengard!\n")
		dialog.speaker()
		dialog.speak("An Ent came to your help?! Ahah! you are truly lucky my friend, for the tree-keepers rarely show themselves to ousiders, let alone help them. Again I must thank you for your help, Saruman will have to be delt with, but for now we must prepare. I suggest you rest for a while and then go to the land of Lothlorien to see Lady Galadriel, to the north east of Isengard, once you pass the Gap of Rohan.\n")
		dialog.speaker("You")
		dialog.speak("#GGood bye master, I shall be ready when you require my help again!\n")
		dialog.stop(true)
		change_quest_status(QUEST_ISENGARD_INVESTIGATE, QUEST_STATUS_FINISHED)
		message(color.YELLOW_GREEN, "Elrond touches your forehead. You feel much better!")
		message()
		inc_stat(A_STR) inc_stat(A_STR)
		inc_stat(A_INT) inc_stat(A_INT)
		inc_stat(A_WIS) inc_stat(A_WIS)
		inc_stat(A_DEX) inc_stat(A_DEX)
		inc_stat(A_CON) inc_stat(A_CON)
		inc_stat(A_CHR) inc_stat(A_CHR)
		return
	end
	dialog.stop()
end
----------------------------------------------------------
------------------------ Khazad-dum ----------------------
----------------------------------------------------------
dialogs.BALIN = function()
	message("Implement me!")
end

----------------------------------------------------------
--------------------------- Misc -------------------------
----------------------------------------------------------
dialogs.FANGORN_QUEST_FIRES_OF_ISENGARD = function()
	dialog.start("Fangorn")
	dialog.speaker()
	dialog.speak("Ahh young one, I nearly mistook your for one of those .. burarum .. those orcs!\nYou should not be here, it is dangerous for someone as short as you are. Isengard power is rising, those burarum, those orcs are coming from there.\nSaruman was once a wise and nice wizard, but it seems his mind is not as pure as I thought. He has a mechanical mind, he burns forests, cuts trees... HHRRRMMM!\n")
	dialog.reply("What... who are you ?")
	dialog()
	dialog.speaker()
	dialog.speak("Hrmmmmm, I am an ent, isn't it obvious ?\n")
	dialog.reply("No, not really... but thanks anyway, I thought I would lay down my last breath in this battle.\n")
	dialog()
	dialog.speaker()
	dialog.speak("You should leave now, we have trees to attend to.\n")
	dialog.reply("Thanks again!")
	dialog()
	dialog.stop()

	change_quest_status(QUEST_ISENGARD_INVESTIGATE, QUEST_STATUS_COMPLETED)
	dun_level = 0
	change_wild_mode()
	player.py = player.wilderness_y
	player.px = player.wilderness_x
end

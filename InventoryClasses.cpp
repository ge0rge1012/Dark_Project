#include "InventoryClasses.h"

extern FontHolder font_holder;
extern TextureHolder texture_holder;
extern settings mysetts;
extern sf::View g_view;

Textures::ID InvItem::get_id()
{
	return id;
}

InvItem::InvItem() {
}

void InvItem::set_item_id(Textures::ID id) {
	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.4, 0.4);

	this->id = id;

	item_type = texture_holder.get_type(id);
}

void InvItem::set_sprite(sf::Sprite new_sprite) {
	sprite = new_sprite;
}

InvItem::InvItem(Textures::ID id) : id(id)
{
	amount = 0;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.4, 0.4);

	this->id = id;

	item_type = texture_holder.get_type(id);
}

void InvItem::set_amount(int amount) {
	this->amount = amount;
}

int InvItem::get_amount()
{
	return amount;
}

void InvItem::add_one()
{
	amount++;
}

void InvItem::set_position(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void InvItem::drawU(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void InvItem::substract_one()
{
	amount--;
}

void InvItem::add_plenty(int num)
{
	amount += num;
}

void InvItem::set_scale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

sf::Vector2f InvItem::get_position()
{
	return sprite.getPosition();
}

InvItem::InvItem(Textures::ID id, int kolvo) : id(id)
{
	amount = kolvo;

	sf::Texture& texture = texture_holder.get(id);
	sprite.setTexture(texture);
	sprite.setScale(0.4, 0.4);
}

int InvItem::get_item_type() {
	return item_type;
}

sf::Sprite InvItem::get_sprite() {
	return sprite;
}

 InvItem& InvItem::operator=(const InvItem& item) {
	 this->id = item.id;
	 this->item_type = item.item_type;
	 this->sprite = item.sprite;
	 this->amount = item.amount;
	 return *this;
}



//____________________________________________________________________

void Inventory::key_reaction(sf::Keyboard::Key key)
{
	if (!inventoryOn)
	{
		int prev_cur = current_item;
		current_item = static_cast<int>(key) - 26;
		if (current_item == 0) current_item = 10;

		for (int j = 20; j < 30; j++)
		{
			if (j == prev_cur)
			{
				inv_items[j].set_scale(sf::Vector2f(0.3, 0.3));
				break;
			}
		}
		

		for (int j = 20; j < 30; j++)
		{
			if (j == current_item)
			{
				inv_items[j].set_scale(sf::Vector2f(0.5, 0.5));
				break;
			}
		}
	}
}

void Inventory::update_statement()
{
	int scr_x = g_view.getCenter().x - mysetts.get_width() / 2;
	int scr_y = g_view.getCenter().y - mysetts.get_height() / 2;

	int pos_x = 108;
	int pos_y = 438;
	for (int i = 20; i < 30; ++i)
	{
		slots[i].setPosition(sf::Vector2f(scr_x + pos_x, scr_y + pos_y));
		pos_x += 43;
	}

	invLine.setPosition(sf::Vector2f(scr_x + 95, scr_y + 430));

	int i = 0;
	for (int j = 20; j < 30; ++j)
	{
		if (i == current_item - 1)
			inv_items[j].set_position(slots[j].getPosition() + sf::Vector2f(5.f, 6.f));
		else
			inv_items[j].set_position(slots[j].getPosition() + sf::Vector2f(12.f, 14.f));

		++i;
		if (i == 10) break;
	}

//__________________________________________

	if (inventoryOn) {
		int back_x = g_view.getCenter().x - 225;
		int back_y = g_view.getCenter().y - 125;

		inventory_sprite.setPosition(back_x, back_y);
		BackgroundGUI.setPosition(inventory_sprite.getPosition().x + 148,
			inventory_sprite.getPosition().y + 10);

		int guiBackX = BackgroundGUI.getPosition().x;
		int guiBackY = BackgroundGUI.getPosition().y + 3;

		for (int i = 0; i < 2; i++) { //right code for slots
			guiBackX = BackgroundGUI.getPosition().x + 15;
			for (int j = 0; j < 5; j++) {
				craftSlots[i * 5 + j].setPosition(guiBackX, guiBackY);
				guiBackX += 52;
			}
			guiBackY += 43;
		}

		for (int i = 0; i < 10; i++) {
			craftItems[i].setPosition(craftSlots[i].getPosition() + sf::Vector2f(8.f, 8.f));
		}

		guiBackY = BackgroundGUI.getPosition().y + 3;
		for (int i = 0; i < 2; i++) { //right code for slots
			guiBackX = BackgroundGUI.getPosition().x + 15;
			for (int j = 0; j < 5; j++) {
				boxSlots[i * 5 + j].setPosition(guiBackX, guiBackY);
				guiBackX += 52;
			}
			guiBackY += 43;
		}

		for (int i = 0; i < 10; i++) {
			boxItems[i].set_position(boxSlots[i].getPosition() + sf::Vector2f(10.f, 12.f));
		}

		guiBackX = BackgroundGUI.getPosition().x+74;
		guiBackY = BackgroundGUI.getPosition().y+21;

		arrow.setPosition(guiBackX + 44, guiBackY + 3);

		for (int i = 0; i < 2; i++) {
			bakeSlots[i].setPosition(guiBackX, guiBackY);
			guiBackX += 80;
		}

		for (int i = 0; i < 2; i++) {
			bakeItems[i].set_position(bakeSlots[i].getPosition() + sf::Vector2f(12.f, 14.f));
		}

		int slot_x = back_x + 14;
		int slot_y = back_y + 111;

		//for (int i = 0; i < 3; i++) { //some shitcode of right positioning inventory slots, cz our designer fucked up with sizes.
		//	slot_x = back_x + 13;
		//	for (int j = 0; j < 10; j++) {
		//		if ((i * 10 + j > 0 && i * 10 + j < 9) || (i * 10 + j > 10 && i * 10 + j < 19) || i * 10 + j==20)
		//			slots[i * 10 + j].setPosition(slot_x + 1, slot_y);
		//		else if (i*10+j>20 && i*10+j<30)
		//			slots[i * 10 + j].setPosition(slot_x+2, slot_y);
		//		else 
		//			slots[i * 10 + j].setPosition(slot_x, slot_y);
		//		slot_x += 43;
		//	}
		//	if (i == 0) slot_y += 43;
		//	else slot_y += 53;
		//}

		for (int i = 0; i < 3; i++) { //right code for slots
			slot_x = back_x + 13;
			for (int j = 0; j < 10; j++) {
				slots[i * 10 + j].setPosition(slot_x, slot_y);
				slot_x += 43;
			}
			if (i == 0) slot_y += 43;
			else slot_y += 51;
		}

		slot_y = back_y + 10;

		for (int i = 0; i < 2; i++) {
			slot_x = back_x + 31;
			for (int j = 0; j < 2; j++) {
				armor_slots[i * 2 + j].setPosition(slot_x, slot_y);
				slot_x += 43;
			}
			slot_y += 43;
		}

		for (int i = 0; i < 30; i++) {
			inv_items[i].set_position(slots[i].getPosition() + sf::Vector2f(12.f, 14.f));
		}
		
		if (isItemOptionsOn()) {
			itemOptionsSprite.setPosition(slots[optionsSlot].getPosition());
		}
	}
}


void Inventory::setOptionsSlot(int slot) {
	optionsSlot = slot;
}

int Inventory::getOptionsSlot() {
	return optionsSlot;
}

void Inventory::decrease_item()
{
	 if (inv_items[current_item + 19].get_amount() == 0) return;

	inv_items[current_item + 19].substract_one();
	updateCrafts();
}

int Inventory::set_current(int num)
{
	return 1;
}

Textures::ID Inventory::get_current()
{
	if (current_item > 10 || current_item < 1 || inv_items[current_item + 19].get_amount() == 0)
		return Textures::ID::NUL;

	return inv_items[current_item + 19].get_id();
}

Inventory::Inventory() 
{
	
	sf::Texture& lineTexture = texture_holder.get(Textures::LINE); 
	invLine.setTexture(lineTexture);

// _______________________________________________________________________
	sf::Texture& invTexture = texture_holder.get(Textures::INVENTORY);
	inventory_sprite.setTexture(invTexture);

	sf::Texture& textureGUI = texture_holder.get(Textures::GUIBACK);
	BackgroundGUI.setTexture(textureGUI);

	sf::Texture& texture_slot = texture_holder.get(Textures::SLOT);
	for (int i = 0; i < 30; i++) {
		slots[i].setTexture(texture_slot);
	}

	//for (int i = 0; i < 2; i++) { //tried to make bakeslots as Sprite
	//	bakeSlots[i].setTexture(texture_slot);
	//}
	
	arrow.setTexture(texture_holder.get(Textures::ARROW));

	for (int i = 0; i < 2; i++) {
		bakeSlots[i].setSize(sf::Vector2f(38.f, 38.f));
		bakeSlots[i].setFillColor(sf::Color(120, 83, 32));
		bakeSlots[i].setOutlineColor(sf::Color(255, 255, 255));
		bakeSlots[i].setOutlineThickness(1.f);
	}

	for (int i = 0; i < 2; i++) {
		bakeItems[i].set_item_id(Textures::ID::DIRT);
		bakeItems[i].set_amount(0);
	}

	for (int i = 0; i < 10; i++) {
		boxSlots[i].setSize(sf::Vector2f(33.f, 33.f));
		boxSlots[i].setFillColor(sf::Color(120, 83, 32));
		boxSlots[i].setOutlineColor(sf::Color(0, 0, 0));
		boxSlots[i].setOutlineThickness(1.f);
	}

	for (int i = 0; i < 10; i++) {
		boxItems[i].set_item_id(Textures::ID::DIRT);
		boxItems[i].set_amount(0);
	}

	for (int i = 0; i < 30; i++) {
		inv_items[i].set_item_id(Textures::ID::DIRT);
	}

	for (int i = 0; i < 4; i++) { //отрисовка 30 слотов
		armor_slots[i].setTexture(texture_slot);
	}

	for (int i = 0; i < 10; i++) {
		craftSlots[i].setSize(sf::Vector2f(31.f, 31.f));
		craftSlots[i].setFillColor(sf::Color(120, 83, 32));
		craftSlots[i].setOutlineColor(sf::Color(171, 0, 0));
		craftSlots[i].setOutlineThickness(1.f);
	}

	itemOptionsSprite.setTexture(texture_holder.get(Textures::ITEM_OPTIONS));

	craftItems[0].setTexture(texture_holder.get(Textures::WORKBENCH));
	craftItems[1].setTexture(texture_holder.get(Textures::BOX));
	craftItems[2].setTexture(texture_holder.get(Textures::BAKE));
	craftItems[3].setTexture(texture_holder.get(Textures::STICK));
	craftItems[4].setTexture(texture_holder.get(Textures::SWORD_IR));
	craftItems[5].setTexture(texture_holder.get(Textures::SWORD_OR));
	craftItems[6].setTexture(texture_holder.get(Textures::PICK_TR));
	craftItems[7].setTexture(texture_holder.get(Textures::PICK_ST));
	craftItems[8].setTexture(texture_holder.get(Textures::PICK_IR));
	craftItems[9].setTexture(texture_holder.get(Textures::PICK_OR));
	for (int i = 0; i < 10; i++) {
		craftItems[i].setScale(0.48, 0.48);
	}

}

void Inventory::setTempItem(std::string arr, int slot) {
	if (arr == "invItem") {
		if (inv_items[slot].get_amount() > 0) {
			tempItem = inv_items[slot];
			inv_items[slot].set_amount(0);
		}
	}
	if (arr == "bakeItem") {
		if (bakeItems[slot].get_amount() > 0) {
			tempItem = bakeItems[slot];
			bakeItems[slot].set_amount(0);
		}
	}
	if (arr == "boxItem") {
		if (boxItems[slot].get_amount() > 0) {
			tempItem = boxItems[slot];
			boxItems[slot].set_amount(0);
		}
	}
}

void Inventory::setFromTemp(std::string arr, int slot) { //smart item moving
	if (arr == "invItem") {
		if (tempItem.get_amount() > 0) {
			if (inv_items[slot].get_amount() > 0) {
				InvItem temp;
				temp = inv_items[slot];
				inv_items[slot] = tempItem;
				tempItem = temp;
				in_hand = true;
			}
			else {
				inv_items[slot] = tempItem;
				tempItem.set_amount(0);
			}
		}
	}
	if (arr == "bakeItem") {
		if (tempItem.get_amount() > 0) {
			if (bakeItems[slot].get_amount() > 0) {
				InvItem temp;
				temp = bakeItems[slot];
				bakeItems[slot] = tempItem;
				tempItem = temp;
				in_hand = true;
			}
			else {
				bakeItems[slot] = tempItem;
				tempItem.set_amount(0);
			}
		}
	}
	if (arr == "boxItem") {
		if (tempItem.get_amount() > 0) {
			if (boxItems[slot].get_amount() > 0) {
				InvItem temp;
				temp = boxItems[slot];
				boxItems[slot] = tempItem;
				tempItem = temp;
				in_hand = true;
			}
			else {
				boxItems[slot] = tempItem;
				tempItem.set_amount(0);
			}
		}
	}
}

void Inventory::drawItemOptions(sf::RenderWindow& window) {
	window.draw(itemOptionsSprite);
}

bool Inventory::add_item_fast(Textures::ID id, int kolvo)
{
	for (int i = 20; i < 30; i++) {
		if (inv_items[i].get_id() == id && inv_items[i].get_amount() != 0)
		{
			inv_items[i].add_plenty(kolvo);
			updateCrafts();
			return true;
		}
	}
		for (int i = 20; i < 30; i++) {
			if (inv_items[i].get_id() == Textures::NUL || inv_items[i].get_amount() == 0) {
				inv_items[i].set_item_id(id);
				inv_items[i].set_amount(kolvo);
				updateCrafts();
				return true;
			}
		}
		return false;
}

bool Inventory::add_invent_item(Textures::ID id, int count) {
	
	for (int i = 0; i < 30; i++) {
		if (inv_items[i].get_id() == id && inv_items[i].get_amount() != 0)
		{
			inv_items[i].add_plenty(count);
			updateCrafts();
			return true;
		}
	}
	for (int i = 0; i < 30; i++) {
		if (inv_items[i].get_id() == Textures::NUL || inv_items[i].get_amount() == 0) {
			inv_items[i].set_item_id(id);
			inv_items[i].set_amount(count);
			updateCrafts();
			return true;
		}
	}

	return false;
}

void Inventory::drawGUIBack(sf::RenderWindow& window) {
	window.draw(BackgroundGUI);
}

void Inventory::drawBakeGUI(sf::RenderWindow& window) {
	if (bakeOn) {
		drawGUIBack(window);
		for (int i = 0; i < 2; i++) {
			window.draw(bakeSlots[i]);
		}
		for (int i = 0; i < 2; i++) {
			if (bakeItems[i].get_amount()>0)
				bakeItems[i].drawU(window);
		}

		for (int i = 0; i < 2; i++) { //также отрисовывается циферка с количеством
			if (bakeItems[i].get_amount() > 0) {
				std::string kol = std::to_string(bakeItems[i].get_amount());
				sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
				text.setFillColor(sf::Color::Black);
				text.setPosition(bakeItems[i].get_position() + sf::Vector2f(10.f, -12.f));
				window.draw(text);
			}
		}

		window.draw(arrow);
	}
}

void Inventory::drawChestGUI(sf::RenderWindow& window) {
	if (boxOn) {
		drawGUIBack(window);
		for (int i = 0; i < 10; i++) {
			window.draw(boxSlots[i]);
		}
		for (int i = 0; i < 10; i++) {
			if (boxItems[i].get_amount() > 0)
				boxItems[i].drawU(window);
		}

		for (int i = 0; i < 10; i++) { //также отрисовывается циферка с количеством
			if (boxItems[i].get_amount() > 0) {
				std::string kol = std::to_string(boxItems[i].get_amount());
				sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
				text.setFillColor(sf::Color::Black);
				text.setPosition(boxItems[i].get_position() + sf::Vector2f(10.f, -12.f));
				window.draw(text);
			}
		}
	}
}

void Inventory::drawWorkbenchGUI(sf::RenderWindow& window) {
	if (inventoryOn)
	{
		drawGUIBack(window);
		for (int i = 0; i < 10; i++) {
			window.draw(craftSlots[i]);
		}

		for (int i = 0; i < 10; i++) {
			window.draw(craftItems[i]);
		}
		
	}
}

void Inventory::drawGUI(sf::RenderWindow& window, sf::Vector2f m_position) {
	if (inventoryOn) //если инвентарь включен (меняется булевая при нажатии Е)
	{
		drawInventoryBack(window); //отрисовка бекгрунда (фона инвентаря)
		for (int i = 0; i < 30; i++) { //отрисовка 30 слотов
			window.draw(slots[i]);
		}
		for (int i = 0; i < 4; i++) { //отрисовка 30 слотов
			window.draw(armor_slots[i]);
		}
		for (int i = 0; i < 30; i++) { //если в слоте больше 0 то отрисовывается то что лежит в соответствующем слоте инвентаря
			if (inv_items[i].get_amount() > 0)
				inv_items[i].drawU(window);
		}
		for (int i = 0; i < 30; i++) { //также отрисовывается циферка с количеством
			if (inv_items[i].get_amount() > 0) {
				std::string kol = std::to_string(inv_items[i].get_amount());
				sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
				text.setFillColor(sf::Color::Black);
				text.setPosition(inv_items[i].get_position() + sf::Vector2f(10.f, -12.f));
				window.draw(text);
			}
		}
		if (isItemOptionsOn()) {
			drawItemOptions(window);
		}
		if (workbenchOn)
			drawWorkbenchGUI(window);
		if (bakeOn)
			drawBakeGUI(window);
		if (boxOn)
			drawChestGUI(window);
		if (is_in_hand() && tempItem.get_amount() > 0) {
			tempItem.set_position(m_position - sf::Vector2f(12.f, 12.f));
			tempItem.drawU(window);
			if (tempItem.get_amount() > 0) {
				std::string kol = std::to_string(tempItem.get_amount());
				sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
				text.setFillColor(sf::Color::Black);
				text.setPosition(tempItem.get_position() + sf::Vector2f(10.f, -12.f));
				window.draw(tempItem.get_sprite());
				window.draw(text);
			}
		}
		

	}
	else {
		window.draw(invLine);
		for (int i = 20; i < 30; ++i)
			window.draw(slots[i]);

		for (int i = 20; i < 30; ++i)
		{
			if (inv_items[i].get_amount() <= 0) continue;
			std::string kol = std::to_string(inv_items[i].get_amount());
			sf::Text text(kol, font_holder.get(Fonts::ID::OLD), 8);
			text.setFillColor(sf::Color::Black);
			text.setPosition((inv_items[i]).get_position() + sf::Vector2f(10.f, -12.f));
			window.draw(inv_items[i].get_sprite());
			window.draw(text);
		}
	}
}

bool Inventory::isWorkbenchOn() {
	return workbenchOn;
};

void Inventory::turnWorkbenchOn(bool on) {
	workbenchOn = on;
};

int Inventory::getInvSlotNow(sf::Vector2i m_position) {
	int slotNow = 31;
	if (inventoryOn) {
		for (int i = 0; i < 30; i++) {
			if ((slots[i].getPosition().x < m_position.x) && (slots[i].getPosition().x + 38 > m_position.x)
				&& (slots[i].getPosition().y < m_position.y) && (slots[i].getPosition().y + 38 > m_position.y))
				slotNow = i;
		}
	}
		return slotNow;
}

int Inventory::getChoose(sf::Vector2i m_position) {
	int choose = 3;
	if (isItemOptionsOn()) {
		if ((slots[optionsSlot].getPosition().x < m_position.x) && (slots[optionsSlot].getPosition().x + 38 > m_position.x)
			&& (slots[optionsSlot].getPosition().y < m_position.y) && (slots[optionsSlot].getPosition().y + 19 > m_position.y))
			choose = 1;
		if ((slots[optionsSlot].getPosition().x < m_position.x) && (slots[optionsSlot].getPosition().x + 38 > m_position.x)
			&& (slots[optionsSlot].getPosition().y+19 < m_position.y) && (slots[optionsSlot].getPosition().y + 38 > m_position.y))
			choose = 2;
		return choose;
	}
}

int Inventory::getBoxSlotNow(sf::Vector2i m_position) {
	int slotNow = 11;
	if (boxOn) {
		for (int i = 0; i < 10; i++) {
			if ((boxSlots[i].getPosition().x < m_position.x) &&
				(boxSlots[i].getPosition().x + 38 > m_position.x)
				&& (boxSlots[i].getPosition().y < m_position.y) &&
				(boxSlots[i].getPosition().y + 38 > m_position.y))
				slotNow = i;
		}
	}
	//std::cout << slotNow << std::endl;
	return slotNow;
}

int Inventory::getBakeSlotNow(sf::Vector2i m_position) {
	int slotNow = 3;
	if (bakeOn) {
		for (int i = 0; i < 2; i++) {
			if ((bakeSlots[i].getPosition().x < m_position.x) &&
				(bakeSlots[i].getPosition().x + 38 > m_position.x)
				&& (bakeSlots[i].getPosition().y < m_position.y) &&
				(bakeSlots[i].getPosition().y + 38 > m_position.y))
				slotNow = i;
		}
	}
	return slotNow;
}

int Inventory::getCraftSlotNow(sf::Vector2i m_position) {
	int slotNow = 11;
	for (int i = 0; i < 10; i++) {
		if ((craftSlots[i].getPosition().x < m_position.x) && 
			(craftSlots[i].getPosition().x + 31 > m_position.x)
			&& (craftSlots[i].getPosition().y < m_position.y) && 
			(craftSlots[i].getPosition().y + 31 > m_position.y))
			slotNow = i;
	}
		return slotNow;
}


bool Inventory::inventoryContains(Textures::ID id, int numb) {
	for (int i = 0; i < 30; i++) {
		if ((inv_items[i].get_id() == id) && (inv_items[i].get_amount() >= numb)) {
			return true;
			break;
		}
	}
	return false;
}



void Inventory::remove_invent_item(Textures::ID id, int count) {
	for (int i = 0; i < 30; i++) {
		if (inv_items[i].get_id() == id && inv_items[i].get_amount()>0) {
			//std::cout << "Removing" << id << " " << count << std::endl;
			//std::cout << "was" << inv_items[i].get_amount() << " become " << inv_items[i].get_amount() - count << std::endl;
			inv_items[i].set_amount(inv_items[i].get_amount() - count);
			updateCrafts();
			break;
		}
	}
}

void Inventory::deleteSlotItems(int slot) {
	inv_items[slot].set_amount(0);
	updateCrafts();
}

void Inventory::turnItemOptions(bool on) {
	itemOptionsOn = on;
}

bool Inventory::isBoxOn() {
	return boxOn;
}
void Inventory::turnBoxOn(bool on) {
	boxOn = on;
	if (on == false)
		loadFromOpenedBox();
	if (on == true)
		loadInOpenedBox();
}

bool Inventory::isItemOptionsOn() {
	return itemOptionsOn;
}

bool Inventory::is_slot_empty(int slot) {
	if (inv_items[slot].get_amount() == 0) return true;
	else return false;
}

void Inventory::drawInventoryBack(sf::RenderWindow& window) {
	window.draw(inventory_sprite);
}

void Inventory::turnGUI(bool on) {
	inventoryOn = on;
	if (on == false) {
		turnItemOptions(false);
		turnWorkbenchOn(false);
		turnBakeOn(false);
		turnBoxOn(false);
	}
}

bool Inventory::isBakeOn() {
	return bakeOn;
}

void Inventory::turnBakeOn(bool on) {
	bakeOn = on;
}

bool Inventory::get_invent_on() {
	return inventoryOn;
}

bool Inventory::is_in_hand() {
	return in_hand;
}
void Inventory::turn_in_hand(bool on) {
	in_hand = on;
}

void Inventory::addBoxCoords(sf::Vector2i m_pos) {
	Coordinate coords;
	coords.x = m_pos.y / 32;
	coords.y = m_pos.x / 32;

	BoxItems items;
	InvItem invItem(Textures::DIRT, 0);
	for (int i = 0; i < 10; i++) {
		items.items[i] = invItem;
	}

	boxesCoords.push_back(coords);
	boxesItems.push_back(items);

	std::cout << "added chest at coors: " << boxesCoords[boxesCoords.size()-1].x 
		<< " " << boxesCoords[boxesCoords.size() - 1].y << std::endl;
	std::cout << "with items:" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << boxesItems[boxesCoords.size() - 1].items[i].get_amount() << std::endl;
	}
}

void Inventory::setOpenedBoxID(sf::Vector2i m_pos) {
	for (int i = 0; i < boxesCoords.size(); i++) {
		if (boxesCoords[i].x == m_pos.y / 32 && boxesCoords[i].y == m_pos.x / 32)
			openedBoxID = i;
	}
}

void Inventory::loadInOpenedBox() { //loading items from boxHolder to drawing slots
	for (int i = 0; i < 10; i++) {
		boxItems[i] = boxesItems[openedBoxID].items[i];
	}
}

void Inventory::loadFromOpenedBox() { //loading items from drawing slots to the boxHolder
	for (int i = 0; i < 10; i++) {
		boxesItems[openedBoxID].items[i] = boxItems[i];
	}
}

void Inventory::deleteBox() {

	boxesCoords.erase(boxesCoords.begin()+openedBoxID);

	boxesItems.erase(boxesItems.begin() + openedBoxID);
}

InvItem Inventory::getItemByIterator(int iterator) {
	return boxesItems[openedBoxID].items[iterator];
}

void Inventory::craftItem(int slot) {
	if (slot == 0 && isCraftable(Textures::WORKBENCH))
	{
		add_invent_item(Textures::WORKBENCH);
		remove_invent_item(Textures::WOOD, 4);
	}
	if (slot == 1 && isCraftable(Textures::BOX))
	{
		add_invent_item(Textures::BOX);
		remove_invent_item(Textures::WOOD, 10);
	}
	if (slot == 2 && isCraftable(Textures::BAKE))
	{
		add_invent_item(Textures::BAKE);
		remove_invent_item(Textures::ROCK, 10);
	}
	if (slot == 3 && isCraftable(Textures::STICK))
	{
		add_invent_item(Textures::STICK);
		remove_invent_item(Textures::WOOD, 2);
	}
	if (slot == 4 && isCraftable(Textures::SWORD_IR))
	{
		add_invent_item(Textures::SWORD_IR);
		remove_invent_item(Textures::IRON_ING, 4);
		remove_invent_item(Textures::STICK, 2);
	}
	if (slot == 5 && isCraftable(Textures::SWORD_OR))
	{
		add_invent_item(Textures::SWORD_OR);
		remove_invent_item(Textures::ORICHALCUM_ING, 4);
		remove_invent_item(Textures::STICK, 2);
	}
	if (slot == 6 && isCraftable(Textures::PICK_TR))
	{
		add_invent_item(Textures::PICK_TR);
		remove_invent_item(Textures::WOOD, 4);
		remove_invent_item(Textures::STICK, 2);
	}
	if (slot == 7 && isCraftable(Textures::PICK_ST))
	{
		add_invent_item(Textures::PICK_ST);
		remove_invent_item(Textures::ROCK, 4);
		remove_invent_item(Textures::STICK, 2);
	}
	if (slot == 8 && isCraftable(Textures::PICK_IR))
	{
		add_invent_item(Textures::PICK_IR);
		remove_invent_item(Textures::IRON_ING, 4);
		remove_invent_item(Textures::STICK, 2);
	}
	if (slot == 9 && isCraftable(Textures::PICK_OR))
	{
		add_invent_item(Textures::PICK_OR);
		remove_invent_item(Textures::ORICHALCUM_ING, 4);
		remove_invent_item(Textures::STICK, 2);
	}
}

void Inventory::updateCrafts() {
	if (isCraftable(Textures::WORKBENCH)) //WB
		craftSlots[0].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[0].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::BOX)) //CHEST
		craftSlots[1].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[1].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::BAKE)) {//BAKE
		std::cout << "SETTED WHITE" << std::endl;
		craftSlots[2].setOutlineColor(sf::Color(20, 105, 20));
	}
	else craftSlots[2].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::STICK)) //STICK
		craftSlots[3].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[3].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::SWORD_IR)) //SW_IR
		craftSlots[4].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[4].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::SWORD_OR)) //SW_OR
		craftSlots[5].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[5].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::PICK_TR)) //PC_TR
		craftSlots[6].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[6].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::PICK_ST)) //PC_ST
		craftSlots[7].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[7].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::PICK_IR)) //PC_IR
		craftSlots[8].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[8].setOutlineColor(sf::Color(178, 0, 0));

	if (isCraftable(Textures::PICK_OR)) //PC_OR
		craftSlots[9].setOutlineColor(sf::Color(20, 105, 20));
	else craftSlots[9].setOutlineColor(sf::Color(178, 0, 0));
}

bool Inventory::isCraftable(Textures::ID id) {
	switch (id) {

	case Textures::WORKBENCH:
		return (inventoryContains(Textures::WOOD, 4));
		break;

	case Textures::BOX:
		return (inventoryContains(Textures::WOOD, 10));
		break;

	case Textures::BAKE:
		return (inventoryContains(Textures::ROCK, 10));
		break;

	case Textures::STICK:
		return (inventoryContains(Textures::WOOD, 2));
		break;

	case Textures::SWORD_IR:
		return ((inventoryContains(Textures::STICK, 2) &&
			inventoryContains(Textures::IRON_ING, 4)));
		break;

	case Textures::SWORD_OR:
		return ((inventoryContains(Textures::STICK, 2) &&
			inventoryContains(Textures::ORICHALCUM_ING, 4)));
		break;

	case Textures::PICK_TR:
		return ((inventoryContains(Textures::STICK, 2) &&
			inventoryContains(Textures::WOOD, 4)));
		break;

	case Textures::PICK_ST:
		return ((inventoryContains(Textures::STICK, 2) &&
			inventoryContains(Textures::ROCK, 4)));
		break;

	case Textures::PICK_IR:
		return ((inventoryContains(Textures::STICK, 2) &&
			inventoryContains(Textures::IRON_ING, 4)));
		break;

	case Textures::PICK_OR:
		return ((inventoryContains(Textures::STICK, 2)) &&
			(inventoryContains(Textures::ORICHALCUM_ING, 4)));
		break;

	default: return false;
	}

}
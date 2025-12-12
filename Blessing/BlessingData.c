#include "Blessing.h"
// Additional Common blessings (IDs 11..20) — no statuses or skills
static Blessing common_more[10] = {
    // 11. Steel Skin
    {
        .id = 11, .name = "Steel Skin", .description = "Tougher body",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_DEFENSE, 4.0f, 1 }, { BEF_MAX_HP, 8.0f, 1 } },
        .effectCount = 2, .statusCount = 0
    },
    // 12. Keen Eye
    {
        .id = 12, .name = "Keen Eye", .description = "Sharper aim",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_ACCURACY, 5.0f, 1 }, { BEF_CRIT_CHANCE, 2.0f, 1 } },
        .effectCount = 2, .statusCount = 0
    },
    // 13. Enduring Heart
    {
        .id = 13, .name = "Enduring Heart", .description = "More vitality",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_MAX_HP, 12.0f, 1 } },
        .effectCount = 1, .statusCount = 0
    },
    // 14. Nimble Steps
    {
        .id = 14, .name = "Nimble Steps", .description = "Easier to dodge",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_DODGE, 6.0f, 1 }, { BEF_ACCURACY, 2.0f, 1 } },
        .effectCount = 2, .statusCount = 0
    },
    // 15. Tempered Arms
    {
        .id = 15, .name = "Tempered Arms", .description = "Slightly harder hits",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_DAMAGE_BOOST, 3.0f, 1 } },
        .effectCount = 1, .statusCount = 0
    },
    // 16. Guarded Mind
    {
        .id = 16, .name = "Guarded Mind", .description = "Minor control resist",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_RES_STUN, 8.0f, 1 }, { BEF_RES_SILENCE, 8.0f, 1 } },
        .effectCount = 2, .statusCount = 0
    },
    // 17. Ember Spark
    {
        .id = 17, .name = "Ember Spark", .description = "A touch of flame",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_FIRE_DAMAGE, 2.0f, 1 } },
        .effectCount = 1, .statusCount = 0
    },
    // 18. Frost Nibble
    {
        .id = 18, .name = "Frost Nibble", .description = "A chill to strikes",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_ICE_DAMAGE, 2.0f, 1 } },
        .effectCount = 1, .statusCount = 0
    },
    // 19. Venom Hint
    {
        .id = 19, .name = "Venom Hint", .description = "Faint toxic edge",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_POISON_DAMAGE, 2.0f, 1 } },
        .effectCount = 1, .statusCount = 0
    },
    // 20. Balanced Form
    {
        .id = 20, .name = "Balanced Form", .description = "Rounded basics",
        .rarity = RARITY_COMMON,
        .effects = { { BEF_MAX_HP, 6.0f, 1 }, { BEF_DEFENSE, 3.0f, 1 }, { BEF_DAMAGE_BOOST, 2.0f, 1 } },
        .effectCount = 3, .statusCount = 0
    }
};

// Rare blessings (IDs 21..40) — 3 effects + 1 status
static Blessing rare_blessings[20] = {
    // 21. Razor Focus
    { .id=21, .name="Razor Focus", .description="Precision and power", .rarity=RARITY_RARE,
      .effects={{BEF_CRIT_CHANCE,5.0f,1},{BEF_CRIT_DAMAGE,20.0f,1},{BEF_ARMOR_PEN,5.0f,1}}, .effectCount=3,
      .statuses={{STATUS_STUN,0.0f,12}}, .statusCount=1 },
    // 22. Ember Surge
    { .id=22, .name="Ember Surge", .description="Hotter flames", .rarity=RARITY_RARE,
      .effects={{BEF_FIRE_DAMAGE,5.0f,1},{BEF_DAMAGE_BOOST,4.0f,1},{BEF_ACCURACY,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_BURN,3.0f,20}}, .statusCount=1 },
    // 23. Glacial Edge
    { .id=23, .name="Glacial Edge", .description="Icy lethality", .rarity=RARITY_RARE,
      .effects={{BEF_ICE_DAMAGE,5.0f,1},{BEF_CRIT_CHANCE,3.0f,1},{BEF_DAMAGE_BOOST,3.0f,1}}, .effectCount=3,
      .statuses={{STATUS_FREEZE,0.0f,15}}, .statusCount=1 },
    // 24. Toxic Venin
    { .id=24, .name="Toxic Venin", .description="Potent poison", .rarity=RARITY_RARE,
      .effects={{BEF_POISON_DAMAGE,5.0f,1},{BEF_ARMOR_PEN,6.0f,1},{BEF_ACCURACY,3.0f,1}}, .effectCount=3,
      .statuses={{STATUS_POISON,4.0f,22}}, .statusCount=1 },
    // 25. Bastion
    { .id=25, .name="Bastion", .description="Rising fortress", .rarity=RARITY_RARE,
      .effects={{BEF_DEFENSE,6.0f,1},{BEF_MAX_HP,20.0f,1},{BEF_DAMAGE_REDUCTION,3.0f,1}}, .effectCount=3,
      .statuses={{STATUS_DMG_TAKEN_UP,0.0f,0}}, .statusCount=1 },
    // 26. Phoenix Feather
    { .id=26, .name="Phoenix Feather", .description="Life through fire", .rarity=RARITY_RARE,
      .effects={{BEF_REGEN,4.0f,1},{BEF_FIRE_RES,6.0f,1},{BEF_MAX_HP,16.0f,1}}, .effectCount=3,
      .statuses={{STATUS_BURN,3.0f,18}}, .statusCount=1 },
    // 27. Polar Ward
    { .id=27, .name="Polar Ward", .description="Cold resistance", .rarity=RARITY_RARE,
      .effects={{BEF_ICE_RES,8.0f,1},{BEF_DEFENSE,5.0f,1},{BEF_DODGE,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_FREEZE,0.0f,14}}, .statusCount=1 },
    // 28. Antidote Veins
    { .id=28, .name="Antidote Veins", .description="Toxin ward", .rarity=RARITY_RARE,
      .effects={{BEF_POISON_RES,8.0f,1},{BEF_MAX_HP,14.0f,1},{BEF_REGEN,3.0f,1}}, .effectCount=3,
      .statuses={{STATUS_POISON,3.0f,18}}, .statusCount=1 },
    // 29. Warlord's Poise
    { .id=29, .name="Warlord's Poise", .description="Composed carnage", .rarity=RARITY_RARE,
      .effects={{BEF_CRIT_DAMAGE,25.0f,1},{BEF_DAMAGE_BOOST,5.0f,1},{BEF_ACCURACY,5.0f,1}}, .effectCount=3,
      .statuses={{STATUS_STUN,0.0f,14}}, .statusCount=1 },
    // 30. Wardbreaker
    { .id=30, .name="Wardbreaker", .description="Piercing blows", .rarity=RARITY_RARE,
      .effects={{BEF_ARMOR_PEN,10.0f,1},{BEF_CRIT_CHANCE,4.0f,1},{BEF_DAMAGE_BOOST,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_DEF_DOWN,2.0f,18}}, .statusCount=1 },
    // 31. Twin Fangs
    { .id=31, .name="Twin Fangs", .description="Double threat", .rarity=RARITY_RARE,
      .effects={{BEF_CRIT_CHANCE,4.0f,1},{BEF_POISON_DAMAGE,5.0f,1},{BEF_ACCURACY,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_POISON,4.0f,20}}, .statusCount=1 },
    // 32. Coldsteel
    { .id=32, .name="Coldsteel", .description="Frigid armor", .rarity=RARITY_RARE,
      .effects={{BEF_DEFENSE,7.0f,1},{BEF_ICE_DAMAGE,4.0f,1},{BEF_ICE_RES,8.0f,1}}, .effectCount=3,
      .statuses={{STATUS_FREEZE,0.0f,16}}, .statusCount=1 },
    // 33. Firebrand
    { .id=33, .name="Firebrand", .description="Branded by fire", .rarity=RARITY_RARE,
      .effects={{BEF_FIRE_DAMAGE,6.0f,1},{BEF_CRIT_DAMAGE,18.0f,1},{BEF_ACCURACY,3.0f,1}}, .effectCount=3,
      .statuses={{STATUS_BURN,4.0f,22}}, .statusCount=1 },
    // 34. Safeguard
    { .id=34, .name="Safeguard", .description="Steady defense", .rarity=RARITY_RARE,
      .effects={{BEF_DEFENSE,6.0f,1},{BEF_DODGE,5.0f,1},{BEF_DAMAGE_REDUCTION,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_WEAKEN,2.0f,0}}, .statusCount=1 },
    // 35. Hunter's Mark
    { .id=35, .name="Hunter's Mark", .description="Track and strike", .rarity=RARITY_RARE,
      .effects={{BEF_ACCURACY,7.0f,1},{BEF_CRIT_CHANCE,5.0f,1},{BEF_ARMOR_PEN,6.0f,1}}, .effectCount=3,
      .statuses={{STATUS_VULNERABLE,2.0f,0}}, .statusCount=1 },
    // 36. Adamant Heart
    { .id=36, .name="Adamant Heart", .description="Will of iron", .rarity=RARITY_RARE,
      .effects={{BEF_RES_STUN,15.0f,1},{BEF_RES_SILENCE,15.0f,1},{BEF_MAX_HP,18.0f,1}}, .effectCount=3,
      .statuses={{STATUS_STUN,0.0f,10}}, .statusCount=1 },
    // 37. Venom Ritual
    { .id=37, .name="Venom Ritual", .description="Toxic rites", .rarity=RARITY_RARE,
      .effects={{BEF_POISON_DAMAGE,6.0f,1},{BEF_LIFESTEAL,4.0f,1},{BEF_DAMAGE_BOOST,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_POISON,5.0f,24}}, .statusCount=1 },
    // 38. Frost Oath
    { .id=38, .name="Frost Oath", .description="Chill oath", .rarity=RARITY_RARE,
      .effects={{BEF_ICE_DAMAGE,5.0f,1},{BEF_ICE_RES,10.0f,1},{BEF_DEFENSE,5.0f,1}}, .effectCount=3,
      .statuses={{STATUS_FREEZE,0.0f,18}}, .statusCount=1 },
    // 39. Flame Oath
    { .id=39, .name="Flame Oath", .description="Fiery oath", .rarity=RARITY_RARE,
      .effects={{BEF_FIRE_DAMAGE,5.0f,1},{BEF_FIRE_RES,10.0f,1},{BEF_CRIT_CHANCE,4.0f,1}}, .effectCount=3,
      .statuses={{STATUS_BURN,4.0f,22}}, .statusCount=1 },
    // 40. Duelist Creed
    { .id=40, .name="Duelist Creed", .description="Masterful strikes", .rarity=RARITY_RARE,
      .effects={{BEF_CRIT_CHANCE,6.0f,1},{BEF_CRIT_DAMAGE,22.0f,1},{BEF_ACCURACY,6.0f,1}}, .effectCount=3,
      .statuses={{STATUS_STUN,0.0f,16}}, .statusCount=1 },
};

// Epic blessings (IDs 41..60) — 4 effects + 2 statuses
static Blessing epic_blessings[20] = {
    // 41. Dragonfire Core
    { .id=41, .name="Dragonfire Core", .description="Essence of flame", .rarity=RARITY_EPIC,
      .effects={{BEF_FIRE_DAMAGE,8.0f,1},{BEF_DAMAGE_BOOST,7.0f,1},{BEF_CRIT_DAMAGE,25.0f,1},{BEF_ACCURACY,6.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,6.0f,28},{STATUS_VULNERABLE,3.0f,0}}, .statusCount=2 },
    // 42. Glacier Heart
    { .id=42, .name="Glacier Heart", .description="Core of winter", .rarity=RARITY_EPIC,
      .effects={{BEF_ICE_DAMAGE,8.0f,1},{BEF_DEFENSE,9.0f,1},{BEF_ICE_RES,15.0f,1},{BEF_DAMAGE_REDUCTION,5.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,22},{STATUS_ATK_DOWN,3.0f,0}}, .statusCount=2 },
    // 43. Basilisk Blood
    { .id=43, .name="Basilisk Blood", .description="Venom supreme", .rarity=RARITY_EPIC,
      .effects={{BEF_POISON_DAMAGE,8.0f,1},{BEF_ARMOR_PEN,12.0f,1},{BEF_LIFESTEAL,6.0f,1},{BEF_CRIT_CHANCE,6.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,7.0f,30},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
    // 44. Aegis Prime
    { .id=44, .name="Aegis Prime", .description="Unyielding bulwark", .rarity=RARITY_EPIC,
      .effects={{BEF_DEFENSE,12.0f,1},{BEF_MAX_HP,40.0f,1},{BEF_DAMAGE_REDUCTION,7.0f,1},{BEF_DODGE,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_WEAKEN,3.0f,0}}, .statusCount=2 },
    // 45. Stormpiercer
    { .id=45, .name="Stormpiercer", .description="Pierce all", .rarity=RARITY_EPIC,
      .effects={{BEF_ARMOR_PEN,15.0f,1},{BEF_CRIT_DAMAGE,30.0f,1},{BEF_CRIT_CHANCE,7.0f,1},{BEF_ACCURACY,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,24},{STATUS_VULNERABLE,3.0f,0}}, .statusCount=2 },
    // 46. Sunforged Plate
    { .id=46, .name="Sunforged Plate", .description="Light-forged armor", .rarity=RARITY_EPIC,
      .effects={{BEF_DEFENSE,11.0f,1},{BEF_FIRE_RES,18.0f,1},{BEF_MAX_HP,35.0f,1},{BEF_REFLECT,6.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,5.0f,24},{STATUS_ATK_DOWN,3.0f,0}}, .statusCount=2 },
    // 47. Winter's Grasp
    { .id=47, .name="Winter's Grasp", .description="Seizing cold", .rarity=RARITY_EPIC,
      .effects={{BEF_ICE_DAMAGE,7.0f,1},{BEF_DAMAGE_BOOST,6.0f,1},{BEF_ICE_RES,18.0f,1},{BEF_DODGE,7.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,26},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
    // 48. Plagueheart
    { .id=48, .name="Plagueheart", .description="Rotten core", .rarity=RARITY_EPIC,
      .effects={{BEF_POISON_DAMAGE,9.0f,1},{BEF_DAMAGE_BOOST,6.0f,1},{BEF_LIFESTEAL,7.0f,1},{BEF_ACCURACY,7.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,7.0f,30},{STATUS_WEAKEN,3.0f,0}}, .statusCount=2 },
    // 49. Battle Trance
    { .id=49, .name="Battle Trance", .description="Flow state", .rarity=RARITY_EPIC,
      .effects={{BEF_CRIT_CHANCE,8.0f,1},{BEF_CRIT_DAMAGE,28.0f,1},{BEF_DAMAGE_BOOST,8.0f,1},{BEF_ACCURACY,9.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,20},{STATUS_DMG_TAKEN_UP,2.0f,0}}, .statusCount=2 },
    // 50. Iron Mountain
    { .id=50, .name="Iron Mountain", .description="Immovable", .rarity=RARITY_EPIC,
      .effects={{BEF_MAX_HP,60.0f,1},{BEF_DEFENSE,14.0f,1},{BEF_DAMAGE_REDUCTION,8.0f,1},{BEF_REFLECT,7.0f,1}}, .effectCount=4,
      .statuses={{STATUS_ATK_DOWN,4.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0}}, .statusCount=2 },
    // 51. Starshot
    { .id=51, .name="Starshot", .description="Heavenly aim", .rarity=RARITY_EPIC,
      .effects={{BEF_ACCURACY,12.0f,1},{BEF_CRIT_CHANCE,9.0f,1},{BEF_ARMOR_PEN,12.0f,1},{BEF_DAMAGE_BOOST,7.0f,1}}, .effectCount=4,
      .statuses={{STATUS_VULNERABLE,3.0f,0},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
    // 52. Emberlord
    { .id=52, .name="Emberlord", .description="Master of flame", .rarity=RARITY_EPIC,
      .effects={{BEF_FIRE_DAMAGE,9.0f,1},{BEF_CRIT_DAMAGE,30.0f,1},{BEF_DAMAGE_BOOST,8.0f,1},{BEF_LIFESTEAL,5.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,6.0f,28},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
    // 53. Frostlord
    { .id=53, .name="Frostlord", .description="Master of ice", .rarity=RARITY_EPIC,
      .effects={{BEF_ICE_DAMAGE,9.0f,1},{BEF_DEFENSE,12.0f,1},{BEF_DAMAGE_REDUCTION,7.0f,1},{BEF_ICE_RES,20.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,26},{STATUS_ATK_DOWN,3.0f,0}}, .statusCount=2 },
    // 54. Plaguelord
    { .id=54, .name="Plaguelord", .description="Master of rot", .rarity=RARITY_EPIC,
      .effects={{BEF_POISON_DAMAGE,10.0f,1},{BEF_ARMOR_PEN,14.0f,1},{BEF_ACCURACY,10.0f,1},{BEF_DAMAGE_BOOST,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,8.0f,32},{STATUS_WEAKEN,3.0f,0}}, .statusCount=2 },
    // 55. Vampiric Grace
    { .id=55, .name="Vampiric Grace", .description="Night gift", .rarity=RARITY_EPIC,
      .effects={{BEF_LIFESTEAL,10.0f,1},{BEF_CRIT_CHANCE,7.0f,1},{BEF_DAMAGE_BOOST,7.0f,1},{BEF_REFLECT,6.0f,1}}, .effectCount=4,
      .statuses={{STATUS_WEAKEN,3.0f,0},{STATUS_DMG_TAKEN_UP,2.0f,0}}, .statusCount=2 },
    // 56. Seraphic Aegis
    { .id=56, .name="Seraphic Aegis", .description="Blessed shield", .rarity=RARITY_EPIC,
      .effects={{BEF_MAX_HP,55.0f,1},{BEF_DEFENSE,13.0f,1},{BEF_DAMAGE_REDUCTION,8.0f,1},{BEF_FIRE_RES,18.0f,1}}, .effectCount=4,
      .statuses={{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_WEAKEN,3.0f,0}}, .statusCount=2 },
    // 57. Trickshot
    { .id=57, .name="Trickshot", .description="Impossible angles", .rarity=RARITY_EPIC,
      .effects={{BEF_ACCURACY,12.0f,1},{BEF_CRIT_DAMAGE,28.0f,1},{BEF_CRIT_CHANCE,8.0f,1},{BEF_ARMOR_PEN,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_VULNERABLE,3.0f,0},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
    // 58. Earthshaker
    { .id=58, .name="Earthshaker", .description="Quaking blows", .rarity=RARITY_EPIC,
      .effects={{BEF_DAMAGE_BOOST,10.0f,1},{BEF_CRIT_DAMAGE,32.0f,1},{BEF_MAX_HP,40.0f,1},{BEF_REFLECT,6.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,24},{STATUS_ATK_DOWN,3.0f,0}}, .statusCount=2 },
    // 59. Ghoststep
    { .id=59, .name="Ghoststep", .description="Untouchable", .rarity=RARITY_EPIC,
      .effects={{BEF_DODGE,14.0f,1},{BEF_DEFENSE,9.0f,1},{BEF_DAMAGE_REDUCTION,7.0f,1},{BEF_ACCURACY,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_WEAKEN,3.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0}}, .statusCount=2 },
    // 60. Warbanner
    { .id=60, .name="Warbanner", .description="Rallying power", .rarity=RARITY_EPIC,
      .effects={{BEF_DAMAGE_BOOST,9.0f,1},{BEF_CRIT_CHANCE,7.0f,1},{BEF_MAX_HP,35.0f,1},{BEF_DEFENSE,10.0f,1}}, .effectCount=4,
      .statuses={{STATUS_VULNERABLE,3.0f,0},{STATUS_DEF_DOWN,3.0f,0}}, .statusCount=2 },
};

// Forward-declare legendary skill effects (no-op stubs)
static void skill_inferno_bless(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_ice_prison_bless(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_plague_cloud_bless(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_immortal_shield(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_thunder_wrath(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_shadow_step(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_solar_flare(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_glacial_barrier(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_venom_burst(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }
static void skill_blade_tempest(CharacterSkill* self, CharacterSkill* target) { (void)self; (void)target; }

// Legendary blessings (IDs 61..80) — 4 effects + 3 statuses + 1 skill
static Blessing legendary_blessings[20] = {
    // 61. Inferno Crown
    { .id=61, .name="Inferno Crown", .description="Crown of flames", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_FIRE_DAMAGE,12.0f,1},{BEF_CRIT_DAMAGE,35.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_ACCURACY,10.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,8.0f,35},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Inferno", skill_inferno_bless, 3, 0} },
    // 62. Icebound Throne
    { .id=62, .name="Icebound Throne", .description="Rule of frost", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_ICE_DAMAGE,12.0f,1},{BEF_DEFENSE,16.0f,1},{BEF_DAMAGE_REDUCTION,10.0f,1},{BEF_ICE_RES,25.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,30},{STATUS_ATK_DOWN,4.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0}}, .statusCount=3,
      .skill={"Ice Prison", skill_ice_prison_bless, 4, 0} },
    // 63. Plague Sovereign
    { .id=63, .name="Plague Sovereign", .description="Rotten majesty", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_POISON_DAMAGE,13.0f,1},{BEF_ARMOR_PEN,16.0f,1},{BEF_LIFESTEAL,10.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,9.0f,36},{STATUS_WEAKEN,4.0f,0},{STATUS_VULNERABLE,4.0f,0}}, .statusCount=3,
      .skill={"Plague Cloud", skill_plague_cloud_bless, 3, 0} },
    // 64. Immortal Bulwark
    { .id=64, .name="Immortal Bulwark", .description="Beyond harm", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_INVULNERABLE,1.0f,1},{BEF_DEFENSE,18.0f,1},{BEF_MAX_HP,80.0f,1},{BEF_REFLECT,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_ATK_DOWN,4.0f,0},{STATUS_WEAKEN,4.0f,0}}, .statusCount=3,
      .skill={"Immortal Shield", skill_immortal_shield, 5, 0} },
    // 65. Thunder's Wrath
    { .id=65, .name="Thunder's Wrath", .description="Strike like storm", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_CRIT_CHANCE,10.0f,1},{BEF_CRIT_DAMAGE,40.0f,1},{BEF_ARMOR_PEN,18.0f,1},{BEF_DAMAGE_BOOST,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,32},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Thunder Wrath", skill_thunder_wrath, 3, 0} },
    // 66. Shadowstep Mantle
    { .id=66, .name="Shadowstep Mantle", .description="Fade and strike", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_DODGE,18.0f,1},{BEF_CRIT_CHANCE,10.0f,1},{BEF_DAMAGE_BOOST,11.0f,1},{BEF_ARMOR_PEN,16.0f,1}}, .effectCount=4,
      .statuses={{STATUS_WEAKEN,4.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Shadow Step", skill_shadow_step, 3, 0} },
    // 67. Solar Ascendant
    { .id=67, .name="Solar Ascendant", .description="Sunlit grace", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_MAX_HP,75.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_CRIT_DAMAGE,36.0f,1},{BEF_REGEN,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,7.0f,34},{STATUS_VULNERABLE,4.0f,0},{STATUS_ATK_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Solar Flare", skill_solar_flare, 4, 0} },
    // 68. Glacial Bastion
    { .id=68, .name="Glacial Bastion", .description="Frozen fortress", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_DEFENSE,17.0f,1},{BEF_ICE_RES,25.0f,1},{BEF_DAMAGE_REDUCTION,10.0f,1},{BEF_REFLECT,10.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,30},{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_WEAKEN,4.0f,0}}, .statusCount=3,
      .skill={"Glacial Barrier", skill_glacial_barrier, 4, 0} },
    // 69. Venom Empress
    { .id=69, .name="Venom Empress", .description="Toxic royalty", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_POISON_DAMAGE,12.0f,1},{BEF_LIFESTEAL,10.0f,1},{BEF_ARMOR_PEN,16.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,9.0f,36},{STATUS_WEAKEN,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Venom Burst", skill_venom_burst, 3, 0} },
    // 70. Blade Tempest
    { .id=70, .name="Blade Tempest", .description="Whirling doom", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_CRIT_CHANCE,10.0f,1},{BEF_CRIT_DAMAGE,40.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,32},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Blade Tempest", skill_blade_tempest, 3, 0} },
    // 71. Ember King
    { .id=71, .name="Ember King", .description="Cinder throne", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_FIRE_DAMAGE,12.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_CRIT_DAMAGE,36.0f,1},{BEF_REGEN,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,8.0f,35},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Inferno", skill_inferno_bless, 3, 0} },
    // 72. Frost Queen
    { .id=72, .name="Frost Queen", .description="Winter crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_ICE_DAMAGE,12.0f,1},{BEF_DEFENSE,16.0f,1},{BEF_DAMAGE_REDUCTION,10.0f,1},{BEF_ICE_RES,25.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,30},{STATUS_ATK_DOWN,4.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0}}, .statusCount=3,
      .skill={"Ice Prison", skill_ice_prison_bless, 4, 0} },
    // 73. Plague King
    { .id=73, .name="Plague King", .description="Rot crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_POISON_DAMAGE,13.0f,1},{BEF_ARMOR_PEN,16.0f,1},{BEF_LIFESTEAL,10.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,9.0f,36},{STATUS_WEAKEN,4.0f,0},{STATUS_VULNERABLE,4.0f,0}}, .statusCount=3,
      .skill={"Plague Cloud", skill_plague_cloud_bless, 3, 0} },
    // 74. Adamant Colossus
    { .id=74, .name="Adamant Colossus", .description="Titan skin", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_INVULNERABLE,1.0f,1},{BEF_DEFENSE,18.0f,1},{BEF_MAX_HP,80.0f,1},{BEF_REFLECT,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_ATK_DOWN,4.0f,0},{STATUS_WEAKEN,4.0f,0}}, .statusCount=3,
      .skill={"Immortal Shield", skill_immortal_shield, 5, 0} },
    // 75. Thunderlord
    { .id=75, .name="Thunderlord", .description="Storm crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_CRIT_CHANCE,10.0f,1},{BEF_CRIT_DAMAGE,40.0f,1},{BEF_ARMOR_PEN,18.0f,1},{BEF_DAMAGE_BOOST,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,32},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Thunder Wrath", skill_thunder_wrath, 3, 0} },
    // 76. Shadow Emperor
    { .id=76, .name="Shadow Emperor", .description="Night crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_DODGE,18.0f,1},{BEF_CRIT_CHANCE,10.0f,1},{BEF_DAMAGE_BOOST,11.0f,1},{BEF_ARMOR_PEN,16.0f,1}}, .effectCount=4,
      .statuses={{STATUS_WEAKEN,4.0f,0},{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Shadow Step", skill_shadow_step, 3, 0} },
    // 77. Solar Emperor
    { .id=77, .name="Solar Emperor", .description="Sun crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_MAX_HP,75.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_CRIT_DAMAGE,36.0f,1},{BEF_REGEN,8.0f,1}}, .effectCount=4,
      .statuses={{STATUS_BURN,7.0f,34},{STATUS_VULNERABLE,4.0f,0},{STATUS_ATK_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Solar Flare", skill_solar_flare, 4, 0} },
    // 78. Frost Emperor
    { .id=78, .name="Frost Emperor", .description="Ice crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_DEFENSE,17.0f,1},{BEF_ICE_RES,25.0f,1},{BEF_DAMAGE_REDUCTION,10.0f,1},{BEF_REFLECT,10.0f,1}}, .effectCount=4,
      .statuses={{STATUS_FREEZE,0.0f,30},{STATUS_DMG_TAKEN_UP,0.0f,0},{STATUS_WEAKEN,4.0f,0}}, .statusCount=3,
      .skill={"Glacial Barrier", skill_glacial_barrier, 4, 0} },
    // 79. Venom Emperor
    { .id=79, .name="Venom Emperor", .description="Poison crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_POISON_DAMAGE,12.0f,1},{BEF_LIFESTEAL,10.0f,1},{BEF_ARMOR_PEN,16.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_POISON,9.0f,36},{STATUS_WEAKEN,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Venom Burst", skill_venom_burst, 3, 0} },
    // 80. Tempest Emperor
    { .id=80, .name="Tempest Emperor", .description="Storm crown", .rarity=RARITY_LEGENDARY,
      .effects={{BEF_CRIT_CHANCE,10.0f,1},{BEF_CRIT_DAMAGE,40.0f,1},{BEF_DAMAGE_BOOST,12.0f,1},{BEF_ACCURACY,12.0f,1}}, .effectCount=4,
      .statuses={{STATUS_STUN,0.0f,32},{STATUS_VULNERABLE,4.0f,0},{STATUS_DEF_DOWN,4.0f,0}}, .statusCount=3,
      .skill={"Blade Tempest", skill_blade_tempest, 3, 0} },
};

// =============================================================
// Registry helpers (no generation; static list only)
// =============================================================

static const Blessing* g_all_ptrs[100];
static int g_all_count = 0;
static int g_reg_inited = 0;

static void add_to_registry(const Blessing* b) {
    if (b && g_all_count < (int)(sizeof(g_all_ptrs)/sizeof(g_all_ptrs[0]))) {
        g_all_ptrs[g_all_count++] = b;
    }
}

void blessing_init_registry() {
    if (g_reg_inited) return;
    g_reg_inited = 1;
    g_all_count = 0;

    // Common 1..10 (named)
    add_to_registry(&warrior_strength);
    add_to_registry(&guardian_shield);
    add_to_registry(&swift_footwork);
    add_to_registry(&minor_regen);
    add_to_registry(&lucky_strike);
    add_to_registry(&flame_touch);
    add_to_registry(&frost_bite);
    add_to_registry(&venom_coating);
    add_to_registry(&iron_will);
    add_to_registry(&beginner_fortune);

    // Common 11..20 (array)
    for (int i = 0; i < 10; ++i) add_to_registry(&common_more[i]);

    // Rare 21..40
    for (int i = 0; i < 20; ++i) add_to_registry(&rare_blessings[i]);
    // Epic 41..60
    for (int i = 0; i < 20; ++i) add_to_registry(&epic_blessings[i]);
    // Legendary 61..80
    for (int i = 0; i < 20; ++i) add_to_registry(&legendary_blessings[i]);
}

int blessing_collect_by_rarity(BlessingRarity r, const Blessing* out[], int cap) {
    blessing_init_registry();
    if (!out || cap <= 0) return 0;
    int written = 0;
    for (int i = 0; i < g_all_count && written < cap; ++i) {
        const Blessing* b = g_all_ptrs[i];
        if (b && b->rarity == r) out[written++] = b;
    }
    return written;
}

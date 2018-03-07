#include "stdafx.h"

#include <boost/preprocessor.hpp>

namespace
{
namespace CompileTimeStrings
{

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

template <char...c>
struct StrToType
{
   static const char buf[];
};
template <char...c>
const char StrToType<c...>::buf[] = { c..., 0 };

#define ENUM_CHARS_MACRO(_, i, s) i < sizeof(s) ? s[i] : 0
#define ENUM_CHARS(n, s) BOOST_PP_ENUM(n, ENUM_CHARS_MACRO, s)

void main()
{
#define STR_TO_TYPE(_, n, s) using s = StrToType<ENUM_CHARS(n, BOOST_PP_STRINGIZE(s))>;

   BOOST_PP_SEQ_FOR_EACH(STR_TO_TYPE, 64,
      (name_00)(name_01)(name_02)(name_03)(name_04)(name_05)(name_06)(name_07)(name_08)(name_09)(name_0A)(name_0B)(name_0C)(name_0D)(name_0E)(name_0F)
      (name_10)(name_11)(name_12)(name_13)(name_14)(name_15)(name_16)(name_17)(name_18)(name_19)(name_1A)(name_1B)(name_1C)(name_1D)(name_1E)(name_1F)
      (name_20)(name_21)(name_22)(name_23)(name_24)(name_25)(name_26)(name_27)(name_28)(name_29)(name_2A)(name_2B)(name_2C)(name_2D)(name_2E)(name_2F)
      (name_30)(name_31)(name_32)(name_33)(name_34)(name_35)(name_36)(name_37)(name_38)(name_39)(name_3A)(name_3B)(name_3C)(name_3D)(name_3E)(name_3F)
      (name_40)(name_41)(name_42)(name_43)(name_44)(name_45)(name_46)(name_47)(name_48)(name_49)(name_4A)(name_4B)(name_4C)(name_4D)(name_4E)(name_4F)
      (name_50)(name_51)(name_52)(name_53)(name_54)(name_55)(name_56)(name_57)(name_58)(name_59)(name_5A)(name_5B)(name_5C)(name_5D)(name_5E)(name_5F)
      (name_60)(name_61)(name_62)(name_63)(name_64)(name_65)(name_66)(name_67)(name_68)(name_69)(name_6A)(name_6B)(name_6C)(name_6D)(name_6E)(name_6F)
      (name_70)(name_71)(name_72)(name_73)(name_74)(name_75)(name_76)(name_77)(name_78)(name_79)(name_7A)(name_7B)(name_7C)(name_7D)(name_7E)(name_7F)
      (name_80)(name_81)(name_82)(name_83)(name_84)(name_85)(name_86)(name_87)(name_88)(name_89)(name_8A)(name_8B)(name_8C)(name_8D)(name_8E)(name_8F)
//      (name_90)(name_91)(name_92)(name_93)(name_94)(name_95)(name_96)(name_97)(name_98)(name_99)(name_9A)(name_9B)(name_9C)(name_9D)(name_9E)(name_9F)
//      (name_A0)(name_A1)(name_A2)(name_A3)(name_A4)(name_A5)(name_A6)(name_A7)(name_A8)(name_A9)(name_AA)(name_AB)(name_AC)(name_AD)(name_AE)(name_AF)
//      (name_B0)(name_B1)(name_B2)(name_B3)(name_B4)(name_B5)(name_B6)(name_B7)(name_B8)(name_B9)(name_BA)(name_BB)(name_BC)(name_BD)(name_BE)(name_BF)
//      (name_C0)(name_C1)(name_C2)(name_C3)(name_C4)(name_C5)(name_C6)(name_C7)(name_C8)(name_C9)(name_CA)(name_CB)(name_CC)(name_CD)(name_CE)(name_CF)
//      (name_D0)(name_D1)(name_D2)(name_D3)(name_D4)(name_D5)(name_D6)(name_D7)(name_D8)(name_D9)(name_DA)(name_DB)(name_DC)(name_DD)(name_DE)(name_DF)
//      (name_E0)(name_E1)(name_E2)(name_E3)(name_E4)(name_E5)(name_E6)(name_E7)(name_E8)(name_E9)(name_EA)(name_EB)(name_EC)(name_ED)(name_EE)(name_EF)
//      (name_F0)(name_F1)(name_F2)(name_F3)(name_F4)(name_F5)(name_F6)(name_F7)(name_F8)(name_F9)(name_FA)(name_FB)(name_FC)(name_FD)(name_FE)(name_FF)
   );

   std::cout << name_8F::buf << std::endl;

}

} // namespace CompileTimeStrings
} // namespace

void CompileTimeStrings()
{
   CompileTimeStrings::main();
}
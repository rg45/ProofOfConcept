#include "stdafx.h"

#include <iostream>

namespace
{

template <typename ClassT, typename MemberT>
struct StaticPointerToMemberTraits
{
   using ClassType = ClassT;
   using MemberType = MemberT;
   using PointerToMemberType = MemberType ClassType::*;
   static PointerToMemberType pointerToMember;
};
template <typename ClassT, typename MemberT>
MemberT ClassT::* StaticPointerToMemberTraits<ClassT, MemberT>::pointerToMember = nullptr;

template <typename TagT> using ClassType = typename TagT::ClassType;
template <typename TagT> using MemberType = typename TagT::MemberType;
template <typename TagT> using PointerToMemberType = typename TagT::PointerToMemberType;

template <typename TagT, PointerToMemberType<TagT> pointerToMember>
class PrivateMemberAccessEnabler
{
   PrivateMemberAccessEnabler() { TagT::pointerToMember = pointerToMember; }
   static PrivateMemberAccessEnabler instance;
};
template <typename TagT, PointerToMemberType<TagT> pointerToMember>
PrivateMemberAccessEnabler<TagT, pointerToMember>
PrivateMemberAccessEnabler<TagT, pointerToMember>::instance;

template <typename TagT>
class PrivateMemberAccessor
{
public:
   explicit PrivateMemberAccessor(ClassType<TagT>& obj) : m_obj(&obj) {}

   MemberType<TagT>& operator* () const { return m_obj->*TagT::pointerToMember; }
   MemberType<TagT>* operator-> () const { return &m_obj->*TagT::pointerToMember; }

private:
   ClassType<TagT>* m_obj;
};

#define ENABLE_PIVATE_MEMBER_ACCESS(TagT, ClassT, MemberT, member) \
   struct TagT : StaticPointerToMemberTraits<ClassT, MemberT> { }; \
   template class PrivateMemberAccessEnabler<TagT, &ClassT::member>;

class A
{
public:

   explicit A(int value) : m_value(value) { }

   int GetValue() const { return m_value; }

private:
   int m_value;
};

ENABLE_PIVATE_MEMBER_ACCESS(A_value, A, int, m_value)

} // namespace

void TestPrivateMemberAccess()
{
   A a(1);
   std::cout << "a.GetValue(): " << a.GetValue() << std::endl;
   *PrivateMemberAccessor<A_value>(a) = 42;
   std::cout << "a.GetValue(): " << a.GetValue() << std::endl;
}


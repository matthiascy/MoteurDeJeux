#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPEID_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPEID_HPP

template <typename T>
class FamilyTypeID {
private:
  inline static UInt64 m_count;

public:
  template <typename U>
  inline static UInt64 next()
  {
    static const UInt64 typeID { m_count++ };
    return m_count;
  }

  inline static UInt64 id()
  {
    return m_count;
  }
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPEID_HPP */

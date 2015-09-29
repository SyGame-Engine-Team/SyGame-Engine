/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Object.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Object fundamental type, all classes herit from this, at
//               least indirectly ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Object implementation
inline Object & Object::operator=( const Object & /*rhs*/ ) {
    // implement this in child classes
    return (*this);
}

inline Bool Object::operator==( const Object & rhs ) const {
    // implement this in child classes
    return (this == &rhs);
}
inline Bool Object::operator!=( const Object & rhs ) const {
    // implement this in child classes
    return (this != &rhs);
}

//inline Bool Object::operator<( const Object & rhs ) const {
//    // implement this in child classes
//    return false;
//}
//inline Bool Object::operator>( const Object & rhs ) const {
//    // implement this in child classes
//    return false;
//}
//inline Bool Object::operator<=( const Object & rhs ) const {
//    // implement this in child classes
//    return false;
//}
//inline Bool Object::operator>=( const Object & rhs ) const {
//    // implement this in child classes
//    return false;
//}

inline DWord Object::GetHash() const {
    // implement this in child classes
    return INVALID_OFFSET;
}

inline Void Object::Serialize( const Object & /*objSerializer*/ ) const {
    // implement this in child classes
}

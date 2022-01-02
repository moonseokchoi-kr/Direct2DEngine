#pragma once



class Serializable
{
public:

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void Serialize(Archive& ar, const unsigned int version);
};

template<typename Archive>
inline void Serializable::Serialize(Archive& ar, const unsigned int version)
{

}

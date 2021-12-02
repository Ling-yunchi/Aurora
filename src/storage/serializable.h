#pragma once
class serializable {
public:
	virtual ~serializable() = default;
	virtual void serialize() = 0;
	virtual void unserialize() = 0;
};

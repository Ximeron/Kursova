#ifndef SESSION_H
#define SESSION_H

class Session
{
    public:
        static Session& getInstance()
        {
            static Session instance;
            return instance;
        }

        void setUserId(int id)
        {
            userId = id;
        }

        int getUserId() const
        {
            return userId;
        }

    private:
        Session() : userId(-1) {}
        Session(const Session&) = delete;
        Session& operator=(const Session&) = delete;

        int userId;
};

#endif // SESSION_H

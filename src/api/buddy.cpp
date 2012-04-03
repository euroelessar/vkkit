#include "buddy_p.h"
#include "roster.h"

namespace vk {

Contact::Contact(int id, Client *client) :
    QObject(client),
    d_ptr(new ContactPrivate(this, id, client))
{
}

Contact::Contact(ContactPrivate *data) :
    QObject(data->client),
    d_ptr(data)
{
}

Contact::~Contact()
{
}

int Contact::id() const
{
    return d_func()->id;
}

Client *Contact::client() const
{
    return d_func()->client;
}

QString Contact::photoSource(Contact::PhotoSize size) const
{
    Q_D(const Contact);
    return d->sources.value(size);
}

void Contact::setPhotoSource(const QString &source, Contact::PhotoSize size)
{
    d_func()->sources[size] = source;
    emit photoSourceChanged(source, size);
}

Buddy::Buddy(int id, Client *client) :
    Contact(new BuddyPrivate(this, id, client))
{
}

QString Buddy::firstName() const
{
    return d_func()->firstName;
}

void Buddy::setFirstName(const QString &firtName)
{
    Q_D(Buddy);
    d->firstName = firtName;
    emit firstNameChanged(firtName);
    emit nameChanged(name());
}

QString Buddy::lastName() const
{
    return d_func()->lastName;
}

void Buddy::setLastName(const QString &lastName)
{
    Q_D(Buddy);
    d->lastName = lastName;
    emit lastNameChanged(lastName);
    emit nameChanged(name());
}

bool Buddy::isOnline() const
{
    return d_func()->status != Offline;
}

void Buddy::setOnline(bool set)
{
    Q_D(Buddy);
    d->status = static_cast<decltype(d->status)>(set);
    emit onlineChanged(set);
}

QString Buddy::name() const
{
    Q_D(const Buddy);
    if (d->firstName.isEmpty() && d->lastName.isEmpty())
        return tr("id%1").arg(id());
    else if (d->lastName.isEmpty())
        return d->firstName;
    else
        return d->firstName + ' ' + d->lastName;

}

QStringList Buddy::tags() const
{
	Q_D(const Buddy);
	QStringList tags;
    foreach (auto data, d->tagIdList) {
        int id = data.toInt();
        tags.append(d->client->roster()->tags().value(id, tr("Unknown tag id %1").arg(id)));
	}
    return tags;
}

QString Buddy::activity() const
{
    return d_func()->activity;
}

Buddy::Status Buddy::status() const
{
    return d_func()->status;
}

void Buddy::setStatus(Buddy::Status status)
{
    d_func()->status = status;
    emit statusChanged(status);
}

void Buddy::update(const QStringList &fields)
{
    //TODO
}

Group::Group(int id, Client *client) :
    Contact(new ContactPrivate(this, id, client))
{
}

QString Group::name() const
{
    return d_func()->name;
}

void Group::setName(const QString &name)
{
    d_func()->name = name;
    emit nameChanged(name);
}

void Group::update(const QStringList &fields)
{
    Q_UNUSED(fields);
    emit updateFinished();
}

} // namespace vk

#include "moc_buddy.cpp"

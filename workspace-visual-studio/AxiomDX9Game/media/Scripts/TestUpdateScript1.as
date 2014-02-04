void update()
{
    BaseEditor @editor = getUpdateObject();
    Vector3 pos = editor.getDerivedPosition();

    while(true)
    {
        pos.x += getTimeSinceLastUpdate() * 2.0f;

        editor.setDerivedPosition(pos);

        yield();
    }
}
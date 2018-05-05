package jp.co.aquariumy.audioio;

public enum UnitySendMessageType
{

	// method
    CALL   ("CallFromAndroid"),
    AUDIOIO("AudioIOFromOS"),
    LOG_D  ("LogDFromOS"),
    LOG_W  ("LogWFromOS");

    public String getMethod()
    {
        return method;
    }

    private final String method;

    private UnitySendMessageType(String method)
    {
        this.method	= method;
    }

}


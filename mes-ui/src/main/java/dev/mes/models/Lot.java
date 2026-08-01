package dev.mes.models;

import com.google.gson.annotations.SerializedName;

public class Lot {
    @SerializedName("LOT_ID")
    private String LOT_ID;

    @SerializedName("ROUTE_ID")
    private String ROUTE_ID;

    @SerializedName("OPER_NO")
    private String OPER_NO;

    @SerializedName("STATUS")
    private String STATUS;

    @SerializedName("PRIORITY")
    private String PRIORITY;

    @SerializedName("WAFER_CNT")
    private String WAFER_CNT;

    @SerializedName("WAFER_SIZE")
    private String WAFER_SIZE;

    @SerializedName("CREATE_DATE")
    private String CREATE_DATE;

    @SerializedName("LAST_OPER_DATE")
    private String LAST_OPER_DATE;

    @SerializedName("COMPLETE_DATE")
    private String COMPLETE_DATE;

    public String getLotID()        { return LOT_ID; }
    public String getRouteID()      { return ROUTE_ID; }
    public String getOperNo()       { return OPER_NO; }
    public String getStatus()       { return STATUS; }
    public String getPriority()     { return PRIORITY; }
    public String getWaferCnt()     { return WAFER_CNT; }
    public String getWaferSize()    { return WAFER_SIZE; }
    public String getCreateDate()   { return CREATE_DATE; }
    public String getLastOperDate() { return LAST_OPER_DATE; }
    public String getCompleteDate() { return COMPLETE_DATE; }

    public void setLotID(String LOT_ID)                { this.LOT_ID = LOT_ID; }
    public void setRouteID(String ROUTE_ID)            { this.ROUTE_ID = ROUTE_ID; }
    public void setOperNo(String OPER_NO)              { this.OPER_NO = OPER_NO; }
    public void setStatus(String STATUS)               { this.STATUS = STATUS; }
    public void setPriority(String PRIORITY)           { this.PRIORITY = PRIORITY; }
    public void setWaferCnt(String WAFER_CNT)          { this.WAFER_CNT = WAFER_CNT; }
    public void setWaferSize(String WAFER_SIZE)        { this.WAFER_SIZE = WAFER_SIZE; }
    public void setCreateDate(String CREATE_DATE)      { this.CREATE_DATE = CREATE_DATE; }
    public void setLastOperDate(String LAST_OPER_DATE) { this.LAST_OPER_DATE = LAST_OPER_DATE; }
    public void setCompleteDate(String COMPLETE_DATE)  { this.COMPLETE_DATE = COMPLETE_DATE; }
}
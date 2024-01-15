import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs';
import { DeviceTelemetry } from '../models/device-telemetry';

@Injectable({
  providedIn: 'root'
})
export class DeviceTelemetryService {

  private baseUrl = 'http://localhost:3000/api/telemetry';

  constructor(private http: HttpClient) { }

  getLatestTelemetry(deviceId: string): Observable<DeviceTelemetry[]> {
    const url = `${this.baseUrl}/latest/${deviceId}`;
    return this.http.get<DeviceTelemetry[]>(url);
  }

  getTelemetryDataInRange(deviceId: string, startTS: string, endTS: string): Observable<DeviceTelemetry[]> {
    const url = `${this.baseUrl}/range/${deviceId}`;
    let params = new HttpParams();
    params = params.append('startTS', startTS);
    params = params.append('endTS', endTS);
    return this.http.get<DeviceTelemetry[]>(url, { params: params });
  }
}

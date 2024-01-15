import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Device } from '../models/device.model';

@Injectable({
  providedIn: 'root'
})
export class DeviceService {

  private baseUrl = 'http://localhost:3000/api/devices';

  constructor(private http: HttpClient) { }

  getAllDevices(): Observable<Device[]> {
    return this.http.get<Device[]>(this.baseUrl);
  }

  getDeviceById(id: string): Observable<Device> {
    const url = `${this.baseUrl}/${id}`;
    return this.http.get<Device>(url);
  }

  createDevice(device: Device): Observable<Device> {
    return this.http.post<Device>(this.baseUrl, device);
  }

  updateDevice(id: string, device: Device): Observable<Device> {
    const url = `${this.baseUrl}/${id}`;
    return this.http.put<Device>(url, device);
  }

  deleteDevice(id: string): Observable<any> {
    const url = `${this.baseUrl}/${id}`;
    return this.http.delete(url);
  }
}

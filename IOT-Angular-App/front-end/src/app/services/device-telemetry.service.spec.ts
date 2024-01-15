import { TestBed } from '@angular/core/testing';

import { DeviceTelemetryService } from './device-telemetry.service';

describe('DeviceTelemetryService', () => {
  let service: DeviceTelemetryService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DeviceTelemetryService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
